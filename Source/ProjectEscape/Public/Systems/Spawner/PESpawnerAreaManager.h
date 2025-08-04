#pragma once

#include "CoreMinimal.h"
#include "PESpawnerAreaManager.generated.h"

UCLASS()
class PROJECTESCAPE_API UPESpawnerAreaManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction = "true"))
	static void TriggerSpawnAreaAround(const UObject* WorldContextObject);
};
