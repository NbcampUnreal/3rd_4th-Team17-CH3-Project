#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PESpawnerAreaManager.generated.h"


UCLASS()
class PROJECTESCAPE_API UPESpawnerAreaManager : public UObject
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Triggers all SpawnAreaOnTrigger actors located within <paramref name="Radius"/> from <paramref name="Origin"/>.
	/// If <paramref name="Tag"/> is specified, only SpawnAreaOnTrigger actors with the matching tag will be triggered.
	/// </summary>
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void TriggerSpawnAreaAround(const UObject* WorldContextObject, UPARAM(ref) FVector& Origin, float Radius, FGameplayTag Tag);
};
