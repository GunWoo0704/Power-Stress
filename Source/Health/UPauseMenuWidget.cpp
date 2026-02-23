#include "UPauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::ResumeGame()
{
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    UGameplayStatics::SetGamePaused(GetWorld(), false);
    PC->SetShowMouseCursor(false);

    FInputModeGameOnly InputMode;
    PC->SetInputMode(InputMode);

    RemoveFromParent();
}

void UPauseMenuWidget::QuitGame()
{
    UKismetSystemLibrary::QuitGame(
        GetWorld(), GetOwningPlayer(),
        EQuitPreference::Quit, false
    );
}
