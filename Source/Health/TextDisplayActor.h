// TextDisplayActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "TextDisplayActor.generated.h"

UCLASS()
class HEALTH_API ATextDisplayActor : public AActor
{
    GENERATED_BODY()

public:
    ATextDisplayActor();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    // 위젯 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UWidgetComponent* WidgetComponent;

    // 레벨에서 수정 가능한 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
    FText DisplayText;

    // 블루프린트에서 호출 가능한 텍스트 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "Text")
    void UpdateDisplayText();
};