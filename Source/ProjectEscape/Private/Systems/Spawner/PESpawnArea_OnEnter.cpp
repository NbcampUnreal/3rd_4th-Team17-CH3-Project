#include "Systems/Spawner/PESpawnArea_OnEnter.h"
#include "Components/BoxComponent.h"


APESpawnArea_OnEnter::APESpawnArea_OnEnter()
{
	EnterTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerArea"));
	EnterTriggerBox->SetupAttachment((UPrimitiveComponent *)ActorIcon);

	IsActivated = false;
}

void APESpawnArea_OnEnter::BeginPlay()
{
	Super::BeginPlay();
	EnterTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCompBeginOverlap);
}

void APESpawnArea_OnEnter::OnCompBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (!IsActivated)
	{
		// TODO: Other is PECharacter?
		Spawn();
	}
	IsActivated = true;
}


