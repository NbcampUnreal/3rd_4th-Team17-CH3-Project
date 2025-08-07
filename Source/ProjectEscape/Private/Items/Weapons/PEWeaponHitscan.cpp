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

void APEWeaponHitscan::Use(AActor* Holder)
{
	Super::Use(Holder);

	FPEAttackStats AttackStats;
	// 테스트용 임시 스탯 설정
	AttackStats.AttackRange = 1000.0f;
	AttackStats.DamageAmount = 17.0f;
	AttackStats.CollisionChannel = ECC_Visibility;
	AttackComponent->ExcuteAttack(AttackStats);
}

UPEAttackBaseComponent* APEWeaponHitscan::CreateAttackComponent()
{
	UE_LOG(LogPE, Warning, TEXT("APEWeaponHitscan::CreateAttackComponent() called!!!"));
	return NewObject<UPEAttackHitscanComponent>(this);
}
