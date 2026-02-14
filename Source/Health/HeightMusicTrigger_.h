// HeightMusicTrigger_.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundBase.h"
#include "HeightMusicTrigger_.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HEALTH_API UHeightMusicTrigger_ : public UActorComponent
{
    GENERATED_BODY()

public:
    UHeightMusicTrigger_();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

public:
    // ========== Music Settings ==========

    // BGM Sound to play when falling
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    USoundBase* BGMSound;

    // Minimum falling speed to trigger music (negative Z velocity)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    float MinFallingSpeed = 500.0f;

    // Minimum height to enable music (won't play if below this height)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    float MinimumHeight = 200.0f;

    // Volume multiplier
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float VolumeMultiplier = 1.0f;

    // Fade in duration (seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music", meta = (ClampMin = "0.0"))
    float FadeInDuration = 0.5f;

    // Fade out duration (seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music", meta = (ClampMin = "0.0"))
    float FadeOutDuration = 1.0f;

    // ========== Control Functions ==========

    UFUNCTION(BlueprintCallable, Category = "Music")
    void PlayMusic();

    UFUNCTION(BlueprintCallable, Category = "Music")
    void StopMusic();

    UFUNCTION(BlueprintCallable, Category = "Music")
    bool IsMusicPlaying() const { return bIsMusicPlaying; }

    UFUNCTION(BlueprintCallable, Category = "Music")
    float GetCurrentFallingSpeed() const;

private:
    class UAudioComponent* AudioComponent;
    bool bIsMusicPlaying;
    FVector LastPosition;

    void CheckFalling();
};