// LevelTransitionZone.cpp
#include "LevelTransitionZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

ALevelTransitionZone::ALevelTransitionZone()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root Component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Trigger Box
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 200.0f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetGenerateOverlapEvents(true);

    // Portal Mesh (Optional visual)
    PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
    PortalMesh->SetupAttachment(TriggerBox);
    PortalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    bHasBeenTriggered = false;
}

void ALevelTransitionZone::BeginPlay()
{
    Super::BeginPlay();

    if (TriggerBox)
    {
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionZone::OnOverlapBegin);
        UE_LOG(LogTemp, Log, TEXT("LevelTransitionZone to '%s' initialized"), *TargetLevelName.ToString());
    }
}

void ALevelTransitionZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    // Check if player character
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character || !Character->IsPlayerControlled())
    {
        return;
    }

    // Check if already triggered
    if (bOneTimeUse && bHasBeenTriggered)
    {
        return;
    }

    bHasBeenTriggered = true;

    UE_LOG(LogTemp, Warning, TEXT("Player entered transition zone! Loading level: %s"), *TargetLevelName.ToString());

    // Start transition with delay
    if (TransitionDelay > 0.0f)
    {
        GetWorldTimerManager().SetTimer(
            TransitionTimerHandle,
            this,
            &ALevelTransitionZone::StartLevelTransition,
            TransitionDelay,
            false
        );
    }
    else
    {
        StartLevelTransition();
    }
}

void ALevelTransitionZone::StartLevelTransition()
{
    // Show loading screen if enabled
    if (bShowLoadingScreen)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PC)
        {
            // Create loading screen from widget class  수정된 부분!
            if (LoadingScreenClass)
            {
                UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(PC, LoadingScreenClass);
                if (LoadingWidget)
                {
                    LoadingWidget->AddToViewport(999);
                    UE_LOG(LogTemp, Log, TEXT("Loading screen displayed"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to create loading screen widget!"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No loading screen widget class assigned! Loading level directly."));
            }

            // Disable input during transition
            /*PC->SetInputMode(FInputModeUIOnly());
            PC->bShowMouseCursor = false;*/
        }

        // Wait minimum loading duration before actually loading
        GetWorldTimerManager().SetTimer(
            LoadingTimerHandle,
            this,
            &ALevelTransitionZone::LoadTargetLevel,
            MinLoadingDuration,
            false
        );
    }
    else
    {
        LoadTargetLevel();
    }
}

void ALevelTransitionZone::LoadTargetLevel()
{
    if (TargetLevelName.IsNone())
    {
        UE_LOG(LogTemp, Error, TEXT("LevelTransitionZone: Target level name is not set!"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Loading level: %s"), *TargetLevelName.ToString());

    UGameplayStatics::OpenLevel(GetWorld(), TargetLevelName);
}
