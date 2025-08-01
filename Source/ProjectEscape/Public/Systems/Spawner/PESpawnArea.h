#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PESpawnArea.generated.h"

class UBillboardComponent;
class UBoxComponent;
class UPESpawnDataAsset;

UCLASS()
class PROJECTESCAPE_API APESpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	APESpawnArea();

	UFUNCTION()
	void Spawn();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GIZMO")
	TObjectPtr<UBillboardComponent> ActorIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	TObjectPtr<UBoxComponent> SpawnArea;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	TSubclassOf<UPESpawnDataAsset> SpawnData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	int32 MinSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	int32 MaxSpawnCount;

protected:
	virtual void BeginPlay() override;

	AActor* SelectRandomActorFromData();
	void GetRandomPointInBox(FVector& OUT Location);
	void GetGroundPointUsingRaycast(FVector& OUT Location);
};
