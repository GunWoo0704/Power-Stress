// LevelTransitionZone.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "LevelTransitionZone.generated.h"

UCLASS()
class HEALTH_API ALevelTransitionZone : public AActor
{
    GENERATED_BODY()

public:
    ALevelTransitionZone();

protected:
    virtual void BeginPlay() override;

    // Trigger Box Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* TriggerBox;

    // Visual Mesh (Optional)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* PortalMesh;

    // ========== Level Settings ==========

    // Target level name to load
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition")
    FName TargetLevelName = "/Game/TokyoStylizedEnvironment/Maps/Tokyo";

    // Loading screen widget class  Ãß°¡!
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition")
    TSubclassOf<UUserWidget> LoadingScreenClass;

    // Show loading screen?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition")
    bool bShowLoadingScreen = true;

    // Loading screen duration (minimum time to show loading)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition", meta = (ClampMin = "0.0"))
    float MinLoadingDuration = 2.0f;

    // Delay before transition starts
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition", meta = (ClampMin = "0.0"))
    float TransitionDelay = 0.5f;

    // One time use?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Transition")
    bool bOneTimeUse = true;

    // ========== Visual Settings ==========

    // Portal color
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor PortalColor = FLinearColor::Blue;

private:
    bool bHasBeenTriggered;
    FTimerHandle TransitionTimerHandle;
    FTimerHandle LoadingTimerHandle;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void StartLevelTransition();
    void LoadTargetLevel();
};