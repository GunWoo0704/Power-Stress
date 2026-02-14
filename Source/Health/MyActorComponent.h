// MyActorComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "MyActorComponent.generated.h"

UENUM(BlueprintType)
enum class ESplineMovementMode : uint8
{
    Loop        UMETA(DisplayName = "Loop"),
    PingPong    UMETA(DisplayName = "PingPong"),
    Once        UMETA(DisplayName = "Once"),
    Reverse     UMETA(DisplayName = "Reverse Loop")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HEALTH_API UMyActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMyActorComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

public:
    // Spline Actor to follow
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    AActor* SplineActor;

    // Movement speed (units per second)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
    float MovementSpeed = 200.0f;

    // Movement mode
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    ESplineMovementMode MovementMode = ESplineMovementMode::Loop;

    // Auto start on begin play?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bAutoStart = true;

    // Rotate along spline direction?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bRotateAlongSpline = true;

    // Rotation offset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FRotator RotationOffset = FRotator::ZeroRotator;

    // Start delay (seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
    float StartDelay = 0.0f;

    // Control functions
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartMovement();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ResetPosition();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementSpeed(float NewSpeed);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetCurrentProgress() const { return CurrentDistance; }

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetProgressPercent() const;

private:
    USplineComponent* SplineComponent;
    bool bIsMoving;
    float CurrentDistance;
    bool bMovingForward;
    float SplineLength;
    float DelayTimer;

    void UpdateActorPosition();
};