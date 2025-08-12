// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PEWeaponProjectile.h"

#include "Combat/Components/PEAttackProjectileComponent.h"
#include "Core/PELogChannels.h"

APEWeaponProjectile::APEWeaponProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APEWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

UPEAttackBaseComponent* APEWeaponProjectile::CreateAttackComponent()
{
	UE_LOG(LogPE, Warning, TEXT("APEWeaponHitscan::CreateAttackComponent() called!!!"));
	return NewObject<UPEAttackProjectileComponent>(this);
}
