// WBP_ResultUI.cpp
#include "WBP_ResultUI.h"

void UWBP_ResultUI::SetResultTime(float InElapsedTime)
{
    if (!ResultTimeText)
    {
        return;
    }

    int32 Minutes, Seconds, Milliseconds;
    FormatTime(InElapsedTime, Minutes, Seconds, Milliseconds);

    FString TimeString = FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Milliseconds);
    ResultTimeText->SetText(FText::FromString(TimeString));

    UE_LOG(LogTemp, Warning, TEXT("Result Time: %s"), *TimeString);
}

void UWBP_ResultUI::FormatTime(float InTime, int32& OutMinutes, int32& OutSeconds, int32& OutMilliseconds)
{
    OutMinutes = FMath::FloorToInt(InTime / 60.0f);
    OutSeconds = FMath::FloorToInt(InTime) % 60;
    OutMilliseconds = FMath::FloorToInt((InTime - FMath::FloorToInt(InTime)) * 100.0f);
}