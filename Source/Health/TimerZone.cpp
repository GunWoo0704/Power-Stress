// TimerZone.cpp
#include "TimerZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "WBP_Timer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ATimerZone::ATimerZone()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Root Component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Setup Trigger Box
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetGenerateOverlapEvents(true);

    // Visual Mesh (Optional)
    ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
    ZoneMesh->SetupAttachment(TriggerBox);
    ZoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    bHasBeenTriggered = false;
}

void ATimerZone::BeginPlay()
{
    Super::BeginPlay();

    // Bind Overlap Events
    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATimerZone::OnOverlapBegin);
        TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATimerZone::OnOverlapEnd);

        UE_LOG(LogTemp, Warning, TEXT("TimerZone initialized successfully!"));
    }
}

void ATimerZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Something overlapped! Actor: %s"), *OtherActor->GetName());

    // Check if it's a Player Character
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not a Character."));
        return;
    }

    if (!Character->IsPlayerControlled())
    {
        UE_LOG(LogTemp, Warning, TEXT("Not Player Controlled."));
        return;
    }

    // Check if already triggered and one-time use
    if (bOneTimeUse && bHasBeenTriggered)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimerZone already triggered."));
        return;
    }

    // Create Timer Widget if not exists
    if (!TimerWidget && TimerWidgetClass)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            TimerWidget = CreateWidget<UWBP_Timer>(PC, TimerWidgetClass);
            if (TimerWidget)
            {
                TimerWidget->AddToViewport();
                UE_LOG(LogTemp, Warning, TEXT("Timer UI created successfully!"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to create Timer UI!"));
            }
        }
    }

    // Start Timer
    if (TimerWidget)
    {
        TimerWidget->StartTimer();
        bHasBeenTriggered = true;

        UE_LOG(LogTemp, Warning, TEXT("Timer Started!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("TimerWidget is null! Did you set TimerWidgetClass?"));
    }
}

void ATimerZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("Overlap ended: %s"), *OtherActor->GetName());
}