#pragma once

#include "CoreMinimal.h"
#include "Systems/Spawner/PESpawnArea.h"
#include "MyPESpawnArea_OnEnter.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTESCAPE_API AMyPESpawnArea_OnEnter : public APESpawnArea
{
	GENERATED_BODY()
	
public:
	AMyPESpawnArea_OnEnter();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnArea")
	TObjectPtr<UBoxComponent> EnterTriggerBox;

	UFUNCTION()
	void OnCompBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);

protected:
	virtual void BeginPlay() override;
	virtual void OnBeginPlay() override;

	bool IsActivated;
};
