#include "Characters/Enemies/Drop/EnemyDropComponent.h"
#include "Characters/Enemies/Drop/DropDataAsset.h"

UEnemyDropComponent::UEnemyDropComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEnemyDropComponent::Drop()
{
	if (TSubclassOf<AActor> SelectedActorClass = SelectRandomActorFromData())
	{
		if (AActor* Enemy = GetOwner())
		{
			FVector SpawnLocation = Enemy->GetActorLocation();
			FRotator SpawnRotator = Enemy->GetActorRotation();
			FActorSpawnParameters SpawnParams;

			if (UWorld* World = GetWorld())
			{
				AActor* SpawnedActor = World->SpawnActor<AActor>(SelectedActorClass, SpawnLocation, SpawnRotator, SpawnParams);
			}
		}
	}
}

TSubclassOf<AActor> UEnemyDropComponent::SelectRandomActorFromData() const
{
	TSubclassOf<AActor> ActorClassToSpawn = nullptr;
	if (DropData)
	{
		int32 TotalWeight = 0;
		for (const FDropDataElement& Data : DropData->DataList)
		{
			TotalWeight += Data.DropWeight;
		}
		int32 Random = FMath::RandRange(1, TotalWeight);
		int32 Cumulative = 0;
		for (const FDropDataElement& Data : DropData->DataList)
		{
			Cumulative += Data.DropWeight;
			if (Random <= Cumulative)
			{
				ActorClassToSpawn = Data.ActorClassToDrop;
				break;
			}
		}
	}
	return ActorClassToSpawn;
}

