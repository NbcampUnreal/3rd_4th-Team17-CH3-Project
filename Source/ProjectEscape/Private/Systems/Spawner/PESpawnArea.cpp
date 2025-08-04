#include "Systems/Spawner/PESpawnArea.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Systems/Spawner/PESpawnDataAsset.h"

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
		if (TSubclassOf<AActor> SelectedActorClass = SelectRandomActorFromData())
		{
			FVector SpawnLocation;
			GetRandomPointInBox(SpawnLocation);
			GetGroundPointUsingRaycast(SpawnLocation);

			FRotator SpawnRotator = FRotator::ZeroRotator;
			GetRandomYawRotation(SpawnRotator);

			FActorSpawnParameters SpawnParams;

			if (UWorld* World = GetWorld())
			{
				if (AActor* SpawnedActor = World->SpawnActor<AActor>(SelectedActorClass, SpawnLocation, SpawnRotator, SpawnParams))
				{
					// ºÎÈ÷È÷
				}
			}	
		}
	}
}

void APESpawnArea::BeginPlay()
{
	Super::BeginPlay();
	Spawn();
}

TSubclassOf<AActor> APESpawnArea::SelectRandomActorFromData() const
{
	TSubclassOf<AActor> ActorClassToSpawn = nullptr;
	if (SpawnData)
	{
		int32 TotalWeight = 0;
		for (const FPESpawnDataElement& Data : SpawnData->DataList)
		{
			TotalWeight += Data.SpawnWeight;
		}
		int32 Random = FMath::RandRange(1, TotalWeight);
		int32 Cumulative = 0;
		for (const FPESpawnDataElement& Data : SpawnData->DataList)
		{
			Cumulative += Data.SpawnWeight;
			if (Random <= Cumulative)
			{
				ActorClassToSpawn = Data.ActorClassToSpawn;
				break;
			}
		}
	}
	return ActorClassToSpawn;
}

void APESpawnArea::GetRandomPointInBox(FVector& OUT Location) const
{
	FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnArea->GetComponentLocation();

	float RandomX = FMath::FRandRange(-BoxExtent.X, BoxExtent.X);
	float RandomY = FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y);
	float TopZ = BoxExtent.Z;

	FVector Random(RandomX, RandomY, TopZ);
	Location = BoxOrigin + Random;
}

void APESpawnArea::GetGroundPointUsingRaycast(FVector& OUT Location) const
{
}

void APESpawnArea::GetRandomYawRotation(FRotator& OUT Rotator) const
{
	Rotator.Yaw = FMath::FRandRange(-90.0f, 90.0f);
}
