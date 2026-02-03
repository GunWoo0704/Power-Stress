// WBP_Timer.cpp
#include "WBP_Timer.h"

void UWBP_Timer::NativeConstruct()
{
    Super::NativeConstruct();

    ElapsedTime = 0.0f;
    bIsTimerActive = false;
}

void UWBP_Timer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bIsTimerActive)
    {
        ElapsedTime += InDeltaTime;  // 시간 증가!
        UpdateTimerDisplay();
    }
}

void UWBP_Timer::StartTimer()
{
    ElapsedTime = 0.0f;
    bIsTimerActive = true;
    UpdateTimerDisplay();
}

void UWBP_Timer::StopTimer()
{
    bIsTimerActive = false;
}

void UWBP_Timer::ResetTimer()
{
    ElapsedTime = 0.0f;
    bIsTimerActive = false;
    UpdateTimerDisplay();
}

void UWBP_Timer::UpdateTimerDisplay()
{
    if (TimerText)
    {
        int32 Minutes = FMath::FloorToInt(ElapsedTime / 60.0f);
        int32 Seconds = FMath::FloorToInt(ElapsedTime) % 60;
        int32 Milliseconds = FMath::FloorToInt((ElapsedTime - FMath::FloorToInt(ElapsedTime)) * 100.0f);

        FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Milliseconds);
        TimerText->SetText(FText::FromString(TimeString));
    }
}