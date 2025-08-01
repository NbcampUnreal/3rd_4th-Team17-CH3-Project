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
		FVector SpawnLocation;
		AActor* SelectedActor = SelectRandomActorFromData();

		GetRandomPointInBox(SpawnLocation);

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

void APESpawnArea::GetRandomPointInBox(FVector& OUT Location)
{
	FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnArea->GetComponentLocation();

	float RandomX = FMath::FRandRange(-BoxExtent.X, BoxExtent.X);
	float RandomY = FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y);
	float TopZ = BoxExtent.Z;

	FVector Random(RandomX, RandomY, TopZ);
	Location = BoxOrigin + Random;
}

void APESpawnArea::GetGroundPointUsingRaycast(FVector& OUT Location)
{
}
