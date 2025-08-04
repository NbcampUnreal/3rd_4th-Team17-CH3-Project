#include "Systems/Spawner/MyPESpawnArea_OnEnter.h"
#include "Components/BoxComponent.h"
#include "Components/BoxComponent.h"


AMyPESpawnArea_OnEnter::AMyPESpawnArea_OnEnter()
{
	EnterTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	EnterTriggerBox->SetupAttachment((UPrimitiveComponent *)ActorIcon);

	IsActivated = false;
}

void AMyPESpawnArea_OnEnter::BeginPlay()
{
	Super::BeginPlay();
	EnterTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCompBeginOverlap);
}

void AMyPESpawnArea_OnEnter::OnBeginPlay()
{
	// Disable Spawn when BeginPlay()
}

void AMyPESpawnArea_OnEnter::OnCompBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (!IsActivated)
	{
		Spawn();
	}
	IsActivated = true;
}


