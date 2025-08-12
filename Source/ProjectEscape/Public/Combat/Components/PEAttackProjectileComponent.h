// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEAttackBaseComponent.h"
#include "PEAttackProjectileComponent.generated.h"


class APEProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEAttackProjectileComponent : public UPEAttackBaseComponent
{
	GENERATED_BODY()

	/* LIfecycle 관련 섹션 */
public:
	UPEAttackProjectileComponent();

protected:
	virtual void BeginPlay() override;

	/* Projectile 공격을 위한 섹션 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<APEProjectileBase> ProjectileClass;
	
protected:
	virtual void PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction) override;

};
