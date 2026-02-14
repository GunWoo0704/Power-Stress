// WBP_ResultUI.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "WBP_ResultUI.generated.h"

UCLASS()
class HEALTH_API UWBP_ResultUI : public UUserWidget
{
    GENERATED_BODY()

public:
    // 위젯 바인딩
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ResultTimeText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TitleText;

    // 시간 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Result")
    void SetResultTime(float InElapsedTime);

private:
    void FormatTime(float InTime, int32& OutMinutes, int32& OutSeconds, int32& OutMilliseconds);
};