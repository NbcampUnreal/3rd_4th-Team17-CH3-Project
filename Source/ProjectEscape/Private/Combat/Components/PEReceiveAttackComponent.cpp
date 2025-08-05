// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEReceiveAttackComponent.h"

#include "Components/BoxComponent.h"
#include "Engine/DamageEvents.h"

UPEReceiveAttackComponent::UPEReceiveAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPEReceiveAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UPEReceiveAttackComponent::ReceiveDamage(float DamageAmount, const FVector& HitLocation, const FVector& HitNormal,
	AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("Received Damage"));
	GetOwner()->TakeDamage(DamageAmount, FDamageEvent(), Instigator->GetInstigatorController(), Instigator);
}
