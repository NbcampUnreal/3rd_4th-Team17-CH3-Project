// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEWeaponBase.h"
#include "PEWeaponProjectile.generated.h"

UCLASS()
class PROJECTESCAPE_API APEWeaponProjectile : public APEWeaponBase
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEWeaponProjectile();

protected:
	virtual void BeginPlay() override;

	/* Projectile 관련 섹션 */	
public:
	virtual UPEAttackBaseComponent* CreateAttackComponent() override;
};
