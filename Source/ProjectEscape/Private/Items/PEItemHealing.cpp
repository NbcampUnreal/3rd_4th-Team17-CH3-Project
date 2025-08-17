// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemHealing.h"

#include "Characters/Hero/PEHero.h"
#include "Characters/Hero/Components/PEInventoryManagerComponent.h"
#include "Core/PELogChannels.h"
#include "Player/PEPlayerState.h"

APEItemHealing::APEItemHealing()
{
	PrimaryActorTick.bCanEverTick = true;
}
void APEItemHealing::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEItemHealing::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearHealingTimer();
}

void APEItemHealing::OnDropped(const FVector& Location, const FRotator& Rotation)
{
	ClearHealingTimer();
	
	Super::OnDropped(Location, Rotation);
}

void APEItemHealing::DoPrimaryAction(AActor* Holder)
{
	if (Holder && !bIsHealing)
	{
		bIsHealing = true;
		bIsReleasedButton = false;
		// 기존 타이머가 있다면 클리어
		if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(HealingTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(HealingTimerHandle);
		}
		
		// ReloadTime 후에 PerformHealing 실행 (Holder를 직접 캡처)
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				HealingTimerHandle,
				[this, Holder]()
				{
					PerformHealing(Holder);
				},
				EquipmentStats.ReloadTime,
				false
			);
			
			UE_LOG(LogPE, Log, TEXT("Healing scheduled in %f seconds"), EquipmentStats.ReloadTime);
		}
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("Holder is null in DoPrimaryAction"));
	}
}

void APEItemHealing::CompletePrimaryAction(AActor* Holder)
{
	bIsHealing = false;
	bIsReleasedButton = true;
}

void APEItemHealing::DoSecondaryAction(AActor* Holder)
{
	
}

void APEItemHealing::DoTertiaryAction(AActor* Holder)
{
	
}

void APEItemHealing::OnHand(AActor* NewOwner)
{
	Super::OnHand(NewOwner);
}

void APEItemHealing::OnRelease()
{
	ClearHealingTimer();

	Super::OnRelease();
}

void APEItemHealing::PerformHealing(AActor* Holder)
{
	if (bIsReleasedButton)
	{
		bIsReleasedButton = false;
		return;
	}
	
	if (Holder)
	{
		if (APEHero* Hero = Cast<APEHero>(Holder))
		{
			Hero->GetPlayerState<APEPlayerState>()->IncreaseHealthPoint(EquipmentStats.Damage);
			if (ItemCount <= 1)
			{
				Hero->DropHandEquipmentToWorld();
			}
			Hero->GetInventoryManagerComponent()->ReduceItemFromInventoryByTag(1, ItemStats.ItemTag);
			UE_LOG(LogPE, Log, TEXT("Healed %s by %d"), *Hero->GetName(), EquipmentStats.Damage);
		}
		else
		{
			UE_LOG(LogPE, Warning, TEXT("Holder is not APEHero"));
		}
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("Holder is null"));
	}
}

void APEItemHealing::ClearHealingTimer()
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(HealingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(HealingTimerHandle);
	}
}
