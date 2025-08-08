// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEReceiveAttackComponent.h"

#include "Core/PELogChannels.h"
#include "Engine/DamageEvents.h"

UPEReceiveAttackComponent::UPEReceiveAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DamageMultiplier = 1.0f;
}

void UPEReceiveAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	DamageTarget = GetOwner();
	
}

void UPEReceiveAttackComponent::ReceiveDamage(float DamageAmount, const FVector& HitLocation, const FVector& HitNormal,
	AActor* Instigator)
{
	if (DamageTarget == nullptr)
	{
		UE_LOG(LogPE, Warning, TEXT("PEReceiveAttackComponent::ReceiveDamage: DamageTarget is null!"));
		return;
	}
	float FinalDamage = DamageAmount * DamageMultiplier;
	FDamageEvent DamageEvent;
	DamageTarget->TakeDamage(FinalDamage, DamageEvent, Instigator->GetInstigatorController(), Instigator);
	UE_LOG(LogTemp, Log, TEXT("PEReceiveAttackComponent::ReceiveDamage: %s received %.2f damage from %s at location %s"),
		*DamageTarget->GetName(), FinalDamage, *Instigator->GetName(), *HitLocation.ToString());
}