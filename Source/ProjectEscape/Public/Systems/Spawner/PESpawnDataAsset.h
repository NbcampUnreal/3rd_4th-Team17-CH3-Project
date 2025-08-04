#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PESpawnDataAsset.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FPESpawnDataElement
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	int32 SpawnWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	TSubclassOf<AActor> ActorClassToSpawn;
};

UCLASS()
class PROJECTESCAPE_API UPESpawnDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnData")
	TArray<FPESpawnDataElement> DataList;
};
