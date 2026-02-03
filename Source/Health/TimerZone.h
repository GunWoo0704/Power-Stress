// TimerZone.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerZone.generated.h"

UCLASS()
class HEALTH_API ATimerZone : public AActor
{
    GENERATED_BODY()

public:
    ATimerZone();

protected:
    virtual void BeginPlay() override;

    // 트리거 박스 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* TriggerBox;

    // 비주얼 메시 (선택사항)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* ZoneMesh;

    // 타이머 UI 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UWBP_Timer> TimerWidgetClass;

    // 한 번만 작동할지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    bool bOneTimeUse = false;

private:
    UPROPERTY()
    UWBP_Timer* TimerWidget;

    bool bHasBeenTriggered;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};