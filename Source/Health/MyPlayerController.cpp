#include "MyPlayerController.h"
#include "UPauseMenuWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

AMyPlayerController::AMyPlayerController()
{
    PauseMenuInstance = nullptr;
    bIsPaused = false;
}

void AMyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (PauseMappingContext)
            Subsystem->AddMappingContext(PauseMappingContext, 0);
    }
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (PauseAction)
            EIC->BindAction(PauseAction, ETriggerEvent::Started,
                this, &AMyPlayerController::TogglePauseMenu);
    }
}

void AMyPlayerController::TogglePauseMenu()
{
    bIsPaused = !bIsPaused;

    if (bIsPaused)
    {
        if (!PauseMenuInstance && PauseMenuWidgetClass)
            PauseMenuInstance = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetClass);

        if (PauseMenuInstance)
        {
            PauseMenuInstance->AddToViewport(10); // ZOrder 높게

            UGameplayStatics::SetGamePaused(GetWorld(), true);
            SetShowMouseCursor(true);

            // GameAndUI: 일시정지 상태에서도 ESC 입력이 다시 들어오게 허용
            FInputModeGameAndUI InputMode;
            InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            SetInputMode(InputMode);
        }
    }
    else
    {
        if (PauseMenuInstance)
            PauseMenuInstance->RemoveFromParent();

        UGameplayStatics::SetGamePaused(GetWorld(), false);
        SetShowMouseCursor(false);

        FInputModeGameOnly InputMode;
        SetInputMode(InputMode);
    }
}
