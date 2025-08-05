// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PEWeaponHitscan.h"

#include "Combat/Components/PEAttackComponentHitscan.h"
#include "Combat/Interface/PEAttackable.h"


APEWeaponHitscan::APEWeaponHitscan()
{
	PrimaryActorTick.bCanEverTick = true;
	AttackComponent = CreateDefaultSubobject<UPEAttackComponentHitscan>(TEXT("AttackComponent"));
	if (IPEAttackable* AttackableInterface = Cast<IPEAttackable>(WeaponOwnerActor))
	{
		AttackComponent->SetAttackStartPoint(AttackableInterface->GetAttackStartPoint());
	}
}

void APEWeaponHitscan::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEWeaponHitscan::Use(AActor* Holder)
{
	Super::Use(Holder);

	AttackComponent->ExcuteAttack();
}
