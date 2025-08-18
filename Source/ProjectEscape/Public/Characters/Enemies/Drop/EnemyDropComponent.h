#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyDropComponent.generated.h"

class UDropDataAsset;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UEnemyDropComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyDropComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop")
	TObjectPtr<UDropDataAsset> DropData;

	void Drop();

protected:
	TSubclassOf<AActor> SelectRandomActorFromData() const;
};
