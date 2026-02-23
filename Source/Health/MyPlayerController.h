// MyPlayerController.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

//  forward declaration 제거하고 직접 include
#include "UPauseMenuWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "MyPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
// class UPauseMenuWidget;  ← 이 줄 삭제

UCLASS()
class HEALTH_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMyPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    UFUNCTION()
    void TogglePauseMenu();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> PauseAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputMappingContext> PauseMappingContext;

    UPROPERTY()
    TObjectPtr<UPauseMenuWidget> PauseMenuInstance;

    bool bIsPaused = false;
};
