// DestructibleMesh.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleMesh.generated.h"

UCLASS()
class HEALTH_API ADestructibleMesh : public AActor
{
    GENERATED_BODY()

public:
    ADestructibleMesh();

protected:
    virtual void BeginPlay() override;

    // Static Mesh Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* MeshComponent;

    // Time before destroying (seconds)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float DestroyDelay = 3.0f;

    // Physics impulse strength when hit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    float ImpulseStrength = 500.0f;

    // Should apply random impulse?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bApplyRandomImpulse = true;

private:
    bool bIsActivated = false;
    FTimerHandle DestroyTimerHandle;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    void DestroyMesh();
};