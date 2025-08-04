#include "Systems/Spawner/PESpawnerAreaManager.h"
#include "Systems/Spawner/PESpawnArea_OnEvent.h"
#include "Kismet/GameplayStatics.h"

void UPESpawnerAreaManager::TriggerSpawnAreaAround(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(WorldContextObject, APESpawnArea_OnEvent::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (APESpawnArea_OnEvent* SpawnArea = Cast<APESpawnArea_OnEvent>(Actor))
			{
				SpawnArea->OnTriggered();
			}
		}
	}
}
