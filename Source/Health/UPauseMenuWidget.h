// PauseMenuWidget.h
#pragma once

//  1. CoreMinimal 먼저
#include "CoreMinimal.h"
//  2. 부모 클래스 헤더
#include "Blueprint/UserWidget.h"
//  3. generated.h는 항상 마지막
#include "UPauseMenuWidget.generated.h"

UCLASS()
class HEALTH_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Pause Menu")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "Pause Menu")
    void QuitGame();
};
