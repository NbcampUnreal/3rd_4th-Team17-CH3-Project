#include "Systems/Spawner/PESpawnArea.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

APESpawnArea::APESpawnArea()
{
	PrimaryActorTick.bCanEverTick = false;

	ActorIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ActorIcon"));
	SetRootComponent(ActorIcon);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(ActorIcon);

	MinSpawnCount = 1;
	MaxSpawnCount = 1;
}

void APESpawnArea::Spawn()
{
	int32 SpawnCount = FMath::RandRange(MinSpawnCount, MaxSpawnCount);
	for (int32 Count = 0; Count < SpawnCount; Count++)
	{
		AActor* SelectedActor = SelectRandomActorFromData();
	}
}

void APESpawnArea::BeginPlay()
{
	Super::BeginPlay();
	Spawn();
}

AActor* APESpawnArea::SelectRandomActorFromData()
{
}
