#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DropDataAsset.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FDropDataElement
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropData")
	int32 DropWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropData")
	TSubclassOf<AActor> ActorClassToDrop;
};

UCLASS()
class PROJECTESCAPE_API UDropDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropData")
	TArray<FDropDataElement> DataList;
};
