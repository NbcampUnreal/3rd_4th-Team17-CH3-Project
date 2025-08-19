// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEProjectileBase.h"
#include "PEProjectileExplosive.generated.h"

UCLASS()
class PROJECTESCAPE_API APEProjectileExplosive : public APEProjectileBase
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEProjectileExplosive();

protected:
	virtual void BeginPlay() override;

	/* 	Explosive 관련 섹션 */
protected:
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION()
	virtual void PerformExplosion();
};
