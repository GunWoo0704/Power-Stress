// WBP_Timer.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WBP_Timer.generated.h"

UCLASS()
class HEALTH_API UWBP_Timer : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    // UI 요소 바인딩
    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimerText;

    // 타이머 함수
    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StartTimer();

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void StopTimer();

    UFUNCTION(BlueprintCallable, Category = "Timer")
    void ResetTimer();

private:
    float ElapsedTime;  // Duration 대신 ElapsedTime으로 변경
    bool bIsTimerActive;

    void UpdateTimerDisplay();
};