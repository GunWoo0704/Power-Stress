// MyActorComponent.cpp
#include "MyActorComponent.h"
#include "Components/SplineComponent.h"

UMyActorComponent::UMyActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    bIsMoving = false;
    CurrentDistance = 0.0f;
    bMovingForward = true;
    SplineLength = 0.0f;
    SplineComponent = nullptr;
    DelayTimer = 0.0f;
}

void UMyActorComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!SplineActor || !IsValid(SplineActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("MyActorComponent on %s: No SplineActor assigned!"),
            *GetOwner()->GetName());
        return;
    }

    SplineComponent = SplineActor->FindComponentByClass<USplineComponent>();

    if (SplineComponent && IsValid(SplineComponent))
    {
        SplineLength = SplineComponent->GetSplineLength();
        UE_LOG(LogTemp, Log, TEXT("MyActorComponent on %s: Found spline, length = %.2f"),
            *GetOwner()->GetName(), SplineLength);

        ResetPosition();

        if (bAutoStart)
        {
            if (StartDelay > 0.0f)
            {
                DelayTimer = StartDelay;
                UE_LOG(LogTemp, Log, TEXT("MyActorComponent: Starting in %.2f seconds"), StartDelay);
            }
            else
            {
                StartMovement();
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MyActorComponent: SplineActor '%s' has no valid SplineComponent!"),
            *SplineActor->GetName());
    }
}

void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsMoving && DelayTimer > 0.0f)
    {
        DelayTimer -= DeltaTime;
        if (DelayTimer <= 0.0f)
        {
            StartMovement();
        }
        return;
    }

    if (!bIsMoving || !SplineComponent || SplineLength <= 0.0f)
    {
        return;
    }

    float DistanceThisFrame = MovementSpeed * DeltaTime;

    if (bMovingForward)
    {
        CurrentDistance += DistanceThisFrame;
    }
    else
    {
        CurrentDistance -= DistanceThisFrame;
    }

    switch (MovementMode)
    {
    case ESplineMovementMode::Loop:
    {
        if (CurrentDistance >= SplineLength)
        {
            CurrentDistance = FMath::Fmod(CurrentDistance, SplineLength);
        }
        break;
    }

    case ESplineMovementMode::PingPong:
    {
        if (CurrentDistance >= SplineLength)
        {
            CurrentDistance = SplineLength;
            bMovingForward = false;
        }
        else if (CurrentDistance <= 0.0f)
        {
            CurrentDistance = 0.0f;
            bMovingForward = true;
        }
        break;
    }

    case ESplineMovementMode::Once:
    {
        if (CurrentDistance >= SplineLength)
        {
            CurrentDistance = SplineLength;
            StopMovement();
            UE_LOG(LogTemp, Log, TEXT("MyActorComponent: Reached end of path"));
        }
        break;
    }

    case ESplineMovementMode::Reverse:
    {
        if (CurrentDistance <= 0.0f)
        {
            CurrentDistance = SplineLength;
        }
        break;
    }
    }

    UpdateActorPosition();
}

void UMyActorComponent::UpdateActorPosition()
{
    if (!SplineComponent || !IsValid(SplineComponent))
    {
        return;
    }

    AActor* Owner = GetOwner();
    if (!Owner || !IsValid(Owner))
    {
        return;
    }

    FVector NewLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
        CurrentDistance,
        ESplineCoordinateSpace::World
    );

    Owner->SetActorLocation(NewLocation);

    if (bRotateAlongSpline)
    {
        FVector Direction = SplineComponent->GetDirectionAtDistanceAlongSpline(
            CurrentDistance,
            ESplineCoordinateSpace::World
        );

        FRotator NewRotation = Direction.Rotation() + RotationOffset;
        Owner->SetActorRotation(NewRotation);
    }
}

void UMyActorComponent::StartMovement()
{
    bIsMoving = true;
    UE_LOG(LogTemp, Log, TEXT("MyActorComponent on %s: Started"), *GetOwner()->GetName());
}

void UMyActorComponent::StopMovement()
{
    bIsMoving = false;
    UE_LOG(LogTemp, Log, TEXT("MyActorComponent on %s: Stopped"), *GetOwner()->GetName());
}

void UMyActorComponent::ResetPosition()
{
    CurrentDistance = 0.0f;
    bMovingForward = true;
    UpdateActorPosition();
    UE_LOG(LogTemp, Log, TEXT("MyActorComponent: Position reset"));
}

void UMyActorComponent::SetMovementSpeed(float NewSpeed)
{
    MovementSpeed = FMath::Max(0.0f, NewSpeed);
    UE_LOG(LogTemp, Log, TEXT("MyActorComponent: Speed set to %.2f"), MovementSpeed);
}

float UMyActorComponent::GetProgressPercent() const
{
    if (SplineLength <= 0.0f)
    {
        return 0.0f;
    }
    return (CurrentDistance / SplineLength) * 100.0f;
}
