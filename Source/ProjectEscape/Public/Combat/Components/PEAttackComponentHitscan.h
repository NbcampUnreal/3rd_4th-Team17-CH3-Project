// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEAttackComponentBase.h"
#include "PEAttackComponentHitscan.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEAttackComponentHitscan : public UPEAttackComponentBase
{
	GENERATED_BODY()

	/* LIfecycle 관련 섹션 */
public:
	UPEAttackComponentHitscan();

protected:
	virtual void BeginPlay() override;

	/* Hitscan 공격을 위한 섹션 */
protected:
	virtual void PerformAttack(const FVector& StartLocation, const FVector& Direction) override;
};
