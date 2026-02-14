// FinishZone.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WBP_Timer.h"
#include "WBP_ResultUI.h"
#include "FinishZone.generated.h"

UCLASS()
class HEALTH_API AFinishZone : public AActor
{
    GENERATED_BODY()

public:
    AFinishZone();

protected:
    virtual void BeginPlay() override;

    // Trigger Box
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* TriggerBox;

    // Visual Mesh (Optional)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* ZoneMesh;

    // ========== UI Settings ==========

    // Timer Widget Reference (WBP_Timer)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UWBP_Timer> TimerWidgetClass;

    // Result UI Widget Class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UWBP_ResultUI> ResultWidgetClass;

    // One Time Use?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bOneTimeUse = true;

private:
    bool bHasBeenTriggered;
    UWBP_ResultUI* ResultWidget;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    // Find existing timer widget in viewport
    UWBP_Timer* FindTimerWidget();
};