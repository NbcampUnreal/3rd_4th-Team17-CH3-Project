// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEProjectileExplosive.h"
#include "PEProjectileDelayedExplosive.generated.h"

UCLASS()
class PROJECTESCAPE_API APEProjectileDelayedExplosive : public APEProjectileExplosive
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEProjectileDelayedExplosive();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* Explosive 관련 섹션 */
protected:
	virtual void Launch(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction) override;

	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY()
	FTimerHandle ExplosionTimerHandle;
};
