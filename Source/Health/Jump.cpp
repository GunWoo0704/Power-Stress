#include "Jump.h"
#include "GameFramework/Character.h"

AJump::AJump()
{
	PrimaryActorTick.bCanEverTick = false;

	// 메시 컴포넌트
	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
	RootComponent = PadMesh;

	// 트리거 박스
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
	TriggerBox->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

void AJump::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AJump::OnOverlapBegin);
}

void AJump::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		FVector LaunchVelocityVector = FVector(0.0f, 0.0f, LaunchVelocity);
		Character->LaunchCharacter(LaunchVelocityVector, bOverrideXY, bOverrideZ);

		UE_LOG(LogTemp, Log, TEXT("JumpPad: Launched %s"), *Character->GetName());
	}
}