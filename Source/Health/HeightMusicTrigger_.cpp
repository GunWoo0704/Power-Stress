// HeightMusicTrigger_.cpp
#include "HeightMusicTrigger_.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UHeightMusicTrigger_::UHeightMusicTrigger_()
{
    PrimaryComponentTick.bCanEverTick = true;

    bIsMusicPlaying = false;
    AudioComponent = nullptr;
    LastPosition = FVector::ZeroVector;
}

void UHeightMusicTrigger_::BeginPlay()
{
    Super::BeginPlay();

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Error, TEXT("HeightMusicTrigger_: No owner!"));
        return;
    }

    LastPosition = Owner->GetActorLocation();

    UE_LOG(LogTemp, Log, TEXT("HeightMusicTrigger_ initialized on %s. Min Falling Speed: %.2f"),
        *Owner->GetName(), MinFallingSpeed);
}

void UHeightMusicTrigger_::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    CheckFalling();
}

void UHeightMusicTrigger_::CheckFalling()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    // Get current position
    FVector CurrentPosition = Owner->GetActorLocation();

    // Check if below minimum height
    if (CurrentPosition.Z < MinimumHeight)
    {
        if (bIsMusicPlaying)
        {
            StopMusic();
        }
        return;
    }

    // Calculate falling speed (negative Z velocity)
    float FallingSpeed = 0.0f;

    // Try to get velocity from Character Movement Component (more accurate)
    ACharacter* Character = Cast<ACharacter>(Owner);
    if (Character && Character->GetCharacterMovement())
    {
        FallingSpeed = -Character->GetCharacterMovement()->Velocity.Z;
    }
    else
    {
        // Fallback: calculate from position change
        FVector PositionDelta = CurrentPosition - LastPosition;
        float TimeDelta = GetWorld()->GetDeltaSeconds();
        if (TimeDelta > 0.0f)
        {
            FallingSpeed = -PositionDelta.Z / TimeDelta;
        }
    }

    LastPosition = CurrentPosition;

    // Check if falling fast enough
    bool bIsFalling = (FallingSpeed >= MinFallingSpeed);

    if (bIsFalling && !bIsMusicPlaying)
    {
        // Started falling
        PlayMusic();
        UE_LOG(LogTemp, Warning, TEXT("Started falling! Speed: %.2f, Height: %.2f"),
            FallingSpeed, CurrentPosition.Z);
    }
    else if (!bIsFalling && bIsMusicPlaying)
    {
        // Stopped falling
        StopMusic();
        UE_LOG(LogTemp, Warning, TEXT("Stopped falling. Speed: %.2f"), FallingSpeed);
    }
}

void UHeightMusicTrigger_::PlayMusic()
{
    if (!BGMSound)
    {
        UE_LOG(LogTemp, Error, TEXT("HeightMusicTrigger_: No BGM Sound assigned!"));
        return;
    }

    if (bIsMusicPlaying)
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }

    // Create or reuse audio component
    if (!AudioComponent || !AudioComponent->IsValidLowLevel())
    {
        AudioComponent = UGameplayStatics::SpawnSound2D(
            GetWorld(),
            BGMSound,
            VolumeMultiplier,
            1.0f,
            0.0f,
            nullptr,
            true,
            false
        );
    }

    if (AudioComponent)
    {
        AudioComponent->SetVolumeMultiplier(VolumeMultiplier);

        if (FadeInDuration > 0.0f)
        {
            AudioComponent->FadeIn(FadeInDuration, VolumeMultiplier);
        }
        else
        {
            AudioComponent->Play();
        }

        bIsMusicPlaying = true;
        UE_LOG(LogTemp, Log, TEXT("HeightMusicTrigger_: Falling music started"));
    }
}

void UHeightMusicTrigger_::StopMusic()
{
    if (!bIsMusicPlaying || !AudioComponent)
    {
        return;
    }

    if (FadeOutDuration > 0.0f)
    {
        AudioComponent->FadeOut(FadeOutDuration, 0.0f);
    }
    else
    {
        AudioComponent->Stop();
    }

    bIsMusicPlaying = false;
    UE_LOG(LogTemp, Log, TEXT("HeightMusicTrigger_: Falling music stopped"));
}

float UHeightMusicTrigger_::GetCurrentFallingSpeed() const
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return 0.0f;
    }

    ACharacter* Character = Cast<ACharacter>(Owner);
    if (Character && Character->GetCharacterMovement())
    {
        return -Character->GetCharacterMovement()->Velocity.Z;
    }

    return 0.0f;
}