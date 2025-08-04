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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	TObjectPtr<UPESpawnDataAsset> SpawnData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	int32 MinSpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	int32 MaxSpawnCount;

protected:
	virtual void BeginPlay() override;

	TSubclassOf<AActor> SelectRandomActorFromData() const;
	void GetRandomPointInBox(FVector& OUT Location) const;
	void GetGroundPointUsingRaycast(FVector& OUT Location) const;
	void GetRandomYawRotation(FRotator& OUT Rotator) const;
};
