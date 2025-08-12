// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEWeaponBase.h"
#include "PEWeaponProjectile.generated.h"

class APEProjectileBase;
struct FPEAttackStats;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<APEProjectileBase> ProjectileClass; // 발사할 Projectile 클래스
	
	virtual UPEAttackBaseComponent* CreateAttackComponent() override;
};
