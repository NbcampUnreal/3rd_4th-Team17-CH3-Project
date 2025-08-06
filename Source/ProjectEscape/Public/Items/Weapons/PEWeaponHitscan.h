// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEWeaponBase.h"
#include "PEWeaponHitscan.generated.h"

class UPEAttackHitscanComponent;

UCLASS()
class PROJECTESCAPE_API APEWeaponHitscan : public APEWeaponBase
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEWeaponHitscan();

protected:
	virtual void BeginPlay() override;

	/* Hitscan 관련 섹션 */	
public:
	virtual void Use(AActor* Holder) override;
	virtual UPEAttackBaseComponent* CreateAttackComponent() override;
};
