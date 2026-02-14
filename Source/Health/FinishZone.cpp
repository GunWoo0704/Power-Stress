// FinishZone.cpp
#include "FinishZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AFinishZone::AFinishZone()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Trigger Box
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetGenerateOverlapEvents(true);

    // Visual Mesh
    ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
    ZoneMesh->SetupAttachment(TriggerBox);
    ZoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    bHasBeenTriggered = false;
    ResultWidget = nullptr;
}

void AFinishZone::BeginPlay()
{
    Super::BeginPlay();

    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFinishZone::OnOverlapBegin);
        UE_LOG(LogTemp, Log, TEXT("FinishZone initialized!"));
    }
}

void AFinishZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어 확인
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character || !Character->IsPlayerControlled())
    {
        return;
    }

    // 이미 작동했는지 확인
    if (bOneTimeUse && bHasBeenTriggered)
    {
        return;
    }

    bHasBeenTriggered = true;
    UE_LOG(LogTemp, Warning, TEXT("Player entered FinishZone!"));

    // 1. 타이머 찾아서 정지
    UWBP_Timer* TimerWidget = FindTimerWidget();

    if (TimerWidget)
    {
        // 타이머 정지
        float FinalTime = TimerWidget->ElapsedTime;
        TimerWidget->StopTimer();

        UE_LOG(LogTemp, Warning, TEXT("Timer stopped! Final time: %.2f seconds"), FinalTime);

        // 타이머 UI 숨기기
        TimerWidget->SetVisibility(ESlateVisibility::Hidden);

        // 2. Result UI 표시
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC && ResultWidgetClass)
        {
            ResultWidget = CreateWidget<UWBP_ResultUI>(PC, ResultWidgetClass);
            if (ResultWidget)
            {
                ResultWidget->AddToViewport(10);
                ResultWidget->SetResultTime(FinalTime);

                // 마우스 커서 표시
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());

                UE_LOG(LogTemp, Warning, TEXT("Result UI displayed! Time: %.2f"), FinalTime);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FinishZone: Timer widget not found!"));
    }
}

UWBP_Timer* AFinishZone::FindTimerWidget()
{
    if (!TimerWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("FinishZone: TimerWidgetClass is not set!"));
        return nullptr;
    }

    // 뷰포트에서 타이머 위젯 찾기
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC)
    {
        return nullptr;
    }

    // 모든 위젯에서 WBP_Timer 찾기
    UWidget* FoundWidget = nullptr;
    TArray<UUserWidget*> FoundWidgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, TimerWidgetClass, false);

    if (FoundWidgets.Num() > 0)
    {
        return Cast<UWBP_Timer>(FoundWidgets[0]);
    }

    UE_LOG(LogTemp, Warning, TEXT("FinishZone: No timer widget found in viewport!"));
    return nullptr;
}