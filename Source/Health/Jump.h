#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"        // 다른 include들이 먼저
#include "Components/StaticMeshComponent.h"
#include "Jump.generated.h"                 // ← 반드시 맨 마지막!

class ACharacter;

UCLASS()
class HEALTH_API AJump : public AActor
{
	GENERATED_BODY()

public:
	AJump();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Pad")
	float LaunchVelocity = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Pad")
	bool bOverrideXY = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump Pad")
	bool bOverrideZ = true;
};