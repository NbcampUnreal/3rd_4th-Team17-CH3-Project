// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEAttackBaseComponent.h"
#include "PEAttackHitscanComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEAttackHitscanComponent : public UPEAttackBaseComponent
{
	GENERATED_BODY()

	/* LIfecycle 관련 섹션 */
public:
	UPEAttackHitscanComponent();

protected:
	virtual void BeginPlay() override;

	/* Hitscan 공격을 위한 섹션 */
protected:
	virtual void PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction) override;
};
