// DestructibleMesh.cpp
#include "DestructibleMesh.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"

ADestructibleMesh::ADestructibleMesh()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create Static Mesh Component
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Setup collision
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
    MeshComponent->SetSimulatePhysics(false); // Initially not simulating
    MeshComponent->SetNotifyRigidBodyCollision(true); // Enable hit events
}

void ADestructibleMesh::BeginPlay()
{
    Super::BeginPlay();

    // Bind hit event
    if (MeshComponent)
    {
        MeshComponent->OnComponentHit.AddDynamic(this, &ADestructibleMesh::OnHit);
        UE_LOG(LogTemp, Warning, TEXT("DestructibleMesh initialized: %s"), *GetName());
    }
}

void ADestructibleMesh::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    // Only activate once
    if (bIsActivated)
    {
        return;
    }

    // Check if hit by player character
    ACharacter* Character = Cast<ACharacter>(OtherActor);
    if (!Character)
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Mesh hit by Character! Starting physics simulation..."));

    // Activate physics simulation
    bIsActivated = true;
    MeshComponent->SetSimulatePhysics(true);
    MeshComponent->SetEnableGravity(true);

    // Apply impulse at hit location
    if (bApplyRandomImpulse)
    {
        FVector ImpulseDirection = FVector(
            FMath::RandRange(-1.0f, 1.0f),
            FMath::RandRange(-1.0f, 1.0f),
            FMath::RandRange(-0.5f, 0.5f)
        ).GetSafeNormal();

        MeshComponent->AddImpulseAtLocation(
            ImpulseDirection * ImpulseStrength,
            Hit.ImpactPoint
        );
    }

    // Set timer to destroy after delay
    GetWorldTimerManager().SetTimer(
        DestroyTimerHandle,
        this,
        &ADestructibleMesh::DestroyMesh,
        DestroyDelay,
        false
    );
}

void ADestructibleMesh::DestroyMesh()
{
    UE_LOG(LogTemp, Warning, TEXT("Destroying mesh: %s"), *GetName());
    Destroy();
}