// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEAttackProjectileComponent.h"


UPEAttackProjectileComponent::UPEAttackProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UPEAttackProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPEAttackProjectileComponent::PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation,
	const FVector& Direction)
{
	
}
