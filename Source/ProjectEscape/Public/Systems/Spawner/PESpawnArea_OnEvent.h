#pragma once

#include "CoreMinimal.h"
#include "Systems/Spawner/PESpawnArea.h"
#include "GameplayTagContainer.h"
#include "PESpawnArea_OnEvent.generated.h"

UCLASS()
class PROJECTESCAPE_API APESpawnArea_OnEvent : public APESpawnArea
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	FGameplayTag SpawnAreaTag;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	void OnTriggered();
};
