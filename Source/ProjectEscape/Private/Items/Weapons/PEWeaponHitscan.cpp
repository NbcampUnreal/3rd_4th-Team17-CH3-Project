// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PEWeaponHitscan.h"

#include "Combat/Components/PEAttackHitscanComponent.h"
#include "Combat/Interface/PEAttackable.h"
#include "Core/PELogChannels.h"


APEWeaponHitscan::APEWeaponHitscan()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APEWeaponHitscan::BeginPlay()
{
	Super::BeginPlay();
	
}

UPEAttackBaseComponent* APEWeaponHitscan::CreateAttackComponent()
{
	UE_LOG(LogPE, Warning, TEXT("APEWeaponHitscan::CreateAttackComponent() called!!!"));
	return NewObject<UPEAttackHitscanComponent>(this);
}
