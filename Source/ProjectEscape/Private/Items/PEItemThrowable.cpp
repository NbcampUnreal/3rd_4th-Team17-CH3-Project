// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemThrowable.h"

#include "Characters/Hero/PEHero.h"
#include "Characters/Hero/Components/PEInventoryManagerComponent.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "Combat/Components/PEAttackProjectileComponent.h"
#include "Core/PELogChannels.h"
#include "Player/PEPlayerController.h"


APEItemThrowable::APEItemThrowable()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComponent = CreateDefaultSubobject<UPEAttackProjectileComponent>(TEXT("AttackComponent"));
}

void APEItemThrowable::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEItemThrowable::Interact(AActor* Interactor)
{
	UE_LOG(LogPE, Warning, TEXT("Interact called on %s by %s"), *GetName(), *Interactor->GetName());
	
	ItemOwnerActor = Interactor;
	if (ItemOwnerActor)
	{
		UE_LOG(LogPE, Log, TEXT("ItemOwnerActor set to %s"), *GetNameSafe(ItemOwnerActor));
	}
	else
	{
		UE_LOG(LogPE, Log, TEXT("ItemOwnerActor is NULL"));
	}
	
	if (IPEAttackable* AttackableInterface = Cast<IPEAttackable>(Interactor))
	{
		AttackComponent->SetAttackStartPoint(AttackableInterface->GetAttackStartPoint());
		UE_LOG(LogPE, Log, TEXT("AttackableInterface found and AttackStartPoint set for %s"), *GetNameSafe(Interactor));
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("Interactor does not implement IPEAttackable interface"));
	}
}

void APEItemThrowable::DoPrimaryAction(AActor* Holder)
{
	if (ItemCount <= 0)
	{
		UE_LOG(LogPE, Warning, TEXT("APEWeaponGranade::DoPrimaryAction: No ammo left!"));
		return;
	}

	if (bIsFiring || bIsReleasedButton)
	{
		return;
	}
	
	// 상태 변수 초기화
	bIsFiring = true;
	
	// 수류탄 투척 시작 - ThrowingReadyTime초 후에 ThrowingReady 호출
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("ThrowingReady"), Holder);
	
	GetWorldTimerManager().SetTimer(
		ThrowingReadyTimerHandle,
		TimerDelegate,
		ThrowingReadyTime,
		false
	);
}

void APEItemThrowable::CompletePrimaryAction(AActor* Holder)
{
	bIsReleasedButton = true;

	if (bIsThrowingReady)
	{
		PerformThrowing(Holder);
	}
}

void APEItemThrowable::DoSecondaryAction(AActor* Holder)
{
	
}

void APEItemThrowable::DoTertiaryAction(AActor* Holder)
{
	
}

void APEItemThrowable::ThrowingReady(AActor* Holder)
{
	bIsThrowingReady = true;
	
	// n초 후에 이미 버튼을 놓은 상태라면 즉시 투척 실행
	if (bIsReleasedButton)
	{
		PerformThrowing(Holder);
	}
}

void APEItemThrowable::PerformThrowing(AActor* Holder)
{
	if (bIsThrowingReady && bIsReleasedButton)
	{
		FPEAttackStats AttackStats;
		AttackStats.AttackRange = EquipmentStats.Range;
		AttackStats.DamageAmount = EquipmentStats.Damage;
		AttackStats.SpreadAngle = EquipmentStats.Spread;
		AttackStats.ProjectileClass = EquipmentStats.ProjectileClass;
		AttackStats.ProjectileSpeed = EquipmentStats.ProjectileSpeed;
		AttackStats.ExplosionRadius = EquipmentStats.ExplosionRadius;
		AttackStats.ExplosionDelay = EquipmentStats.ExplosionDelayTime;
		AttackStats.HitscanChannel = ECC_Visibility; //TODO: Collision 수정 필요

		AttackComponent->ExcuteAttack(AttackStats);
		
		if (APEHero* PEHero = Cast<APEHero>(ItemOwnerActor))
		{
			if (EquipmentStats.IsAutomatic)
			{
				float ShotIntervalSeconds = 60.0f / EquipmentStats.FireRate;
				PEHero->PlayFireWeaponAnimation(ShotIntervalSeconds);
			}
			else
			{
				PEHero->PlayFireWeaponAnimation(0);
			}

			if (APEPlayerController* PEPlayerController = PEHero->GetController<APEPlayerController>())
			{
				PEPlayerController->PlayAimAnimOfHUDWidget();
			}
		}

		UE_LOG(LogPE, Log, TEXT("PerformThrowing: %s has thrown a grenade with stats: Range: %f, Damage: %f, ProjectileSpeed: %f"),
			*GetName(), AttackStats.AttackRange, AttackStats.DamageAmount, AttackStats.ProjectileSpeed);

		
		if (APEHero* Hero = Cast<APEHero>(Holder))
		{
			if (ItemCount <= 1)
			{
				Hero->DropHandEquipmentToWorld();
			}
			Hero->GetInventoryManagerComponent()->ReduceItemFromInventoryByTag(1, ItemStats.ItemTag);
		}

		bIsThrowingReady = false;
		bIsReleasedButton = false;
		bIsFiring = false;
		
		BroadcastEmptyHUDState();
	}
}
