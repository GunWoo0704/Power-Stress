// TextDisplayActor.cpp
#include "TextDisplayActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ATextDisplayActor::ATextDisplayActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // 루트 컴포넌트 생성
    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    // 위젯 컴포넌트 생성
    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(Root);

    // 3D 공간에 표시
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);

    // 기본 크기 설정
    WidgetComponent->SetDrawSize(FVector2D(500.f, 100.f));

    // 기본 텍스트
    DisplayText = FText::FromString(TEXT("기본 텍스트"));
}

void ATextDisplayActor::BeginPlay()
{
    Super::BeginPlay();
    UpdateDisplayText();
}

void ATextDisplayActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    UpdateDisplayText();
}

void ATextDisplayActor::UpdateDisplayText()
{
    if (!WidgetComponent || !WidgetComponent->GetWidget())
        return;

    UUserWidget* Widget = WidgetComponent->GetWidget();
    if (!Widget)
        return;

    // 위젯에서 MainTextBox라는 이름의 TextBlock 찾기
    UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("MainTextBox")));

    if (TextBlock)
    {
        TextBlock->SetText(DisplayText);
    }
}