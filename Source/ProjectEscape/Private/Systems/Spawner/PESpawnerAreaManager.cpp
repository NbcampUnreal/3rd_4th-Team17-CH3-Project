#include "Systems/Spawner/PESpawnerAreaManager.h"
#include "Systems/Spawner/PESpawnArea_OnEvent.h"
#include "Kismet/GameplayStatics.h"

void UPESpawnerAreaManager::TriggerSpawnAreaAround(const UObject* WorldContextObject, UPARAM(ref)FVector& Origin, float Radius, FGameplayTag Tag)
{
	if (WorldContextObject)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(WorldContextObject, APESpawnArea_OnEvent::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (APESpawnArea_OnEvent* SpawnArea = Cast<APESpawnArea_OnEvent>(Actor))
			{
				FVector Delta = Origin - SpawnArea->GetActorLocation();
				bool DistanceCheck = Radius < 0 || Delta.Length() <= Radius;
				bool TagCheck = (Tag == FGameplayTag()) || (Tag.MatchesTag(SpawnArea->SpawnAreaTag));
				if (DistanceCheck && TagCheck)
				{
					SpawnArea->OnTriggered();
				}
			}
		}
	}
}
