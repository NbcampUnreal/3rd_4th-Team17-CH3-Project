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
			FVector SpawnLocation = GetRandomPointInBox();
			FRotator SpawnRotator = GetRandomYawRotation();
			FActorSpawnParameters SpawnParams;

			if (UWorld* World = GetWorld())
			{
				if (AActor* SpawnedActor = World->SpawnActor<AActor>(SelectedActorClass, SpawnLocation, SpawnRotator, SpawnParams))
				{
					PlaceActorOnGround(SpawnedActor);
				}
			}	
		}
	}
}

void APESpawnArea::BeginPlay()
{
	Super::BeginPlay();
	OnBeginPlay();
}

void APESpawnArea::OnBeginPlay()
{
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

FVector APESpawnArea::GetRandomPointInBox() const
{
	FVector BoxExtent = SpawnArea->GetScaledBoxExtent();
	FVector BoxOrigin = SpawnArea->GetComponentLocation();

	float RandomX = FMath::FRandRange(-BoxExtent.X, BoxExtent.X);
	float RandomY = FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y);
	float TopZ = BoxExtent.Z;

	FVector Random(RandomX, RandomY, TopZ);
	return BoxOrigin + Random;
}

FRotator APESpawnArea::GetRandomYawRotation() const
{
	float RandomYaw = FMath::FRandRange(-90.0f, 90.0f);
	return FRotator(0, RandomYaw, 0);
}

void APESpawnArea::PlaceActorOnGround(AActor* SpawnedActor)
{
	check(SpawnedActor);

	FVector BoxOrigin;
	FVector BoxExtent;
	SpawnedActor->GetActorBounds(true, BoxOrigin, BoxExtent);

	FCollisionShape BoxCollision = FCollisionShape::MakeBox(BoxExtent);
	FVector OldLocation = SpawnedActor->GetActorLocation();
	FVector NewLocation = GetGroundPointUsingSweep(SpawnedActor, BoxCollision);
	NewLocation.Z += BoxExtent.Z; // add height/2 of actor to impact point of sweep result
	SpawnedActor->SetActorLocation(NewLocation);
}

FVector APESpawnArea::GetGroundPointUsingSweep(AActor* SpawnedActor, FCollisionShape& BoxCollision) const
{
	check(SpawnedActor);
	check(SpawnArea);

	static const FVector FallDirection = FVector(0, 0, -1.0f);
	static const float DefaultCheckDistance = 1000.0f;
	float BoxHalfHeight = SpawnArea->GetScaledBoxExtent().Z;

	FVector Start = SpawnedActor->GetActorLocation();
	FVector Result = Start;

	if (UWorld* World = GetWorld())
	{
		float CheckDistance = DefaultCheckDistance + (BoxHalfHeight * 2);
		FVector End = Start + FallDirection * (CheckDistance);
		FQuat Rot;
		FHitResult HitResult;
		
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(SpawnedActor);

		if (World->SweepSingleByChannel(HitResult, Start, End, Rot, ECC_Visibility, BoxCollision, Params))
		{
			Result.Z = HitResult.ImpactPoint.Z;
		}
	}
	return Result;
}

