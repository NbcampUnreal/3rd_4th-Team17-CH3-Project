#pragma once

#include "CoreMinimal.h"
#include "Systems/Spawner/PESpawnArea.h"
#include "PESpawnArea_OnEvent.generated.h"

UCLASS()
class PROJECTESCAPE_API APESpawnArea_OnEvent : public APESpawnArea
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void OnTriggered();
};
