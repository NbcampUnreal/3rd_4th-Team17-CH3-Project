// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PEWeaponGranade.h"

#include "Characters/Hero/PEHero.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "Core/PELogChannels.h"
#include "Player/PEPlayerController.h"


APEWeaponGranade::APEWeaponGranade()
{

}

void APEWeaponGranade::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoCount = 4;
}

void APEWeaponGranade::DoPrimaryAction(AActor* Holder)
{
	if (CurrentAmmoCount <= 0)
	{
		UE_LOG(LogPE, Warning, TEXT("APEWeaponGranade::DoPrimaryAction: No ammo left!"));
		return;
	}
	
	// 상태 변수 초기화
	bIsThrowingReady = false;
	bIsReleasedButton = false;
	
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

void APEWeaponGranade::CompletePrimaryAction(AActor* Holder)
{
	bIsReleasedButton = true;

	if (bIsThrowingReady)
	{
		PerformThrowing(Holder);
	}
}

void APEWeaponGranade::DoTertiaryAction(AActor* Holder)
{
	// Note: 수류탄은 재장전 기능이 없으므로 비워둠
}

void APEWeaponGranade::ThrowingReady(AActor* Holder)
{
	bIsThrowingReady = true;
	
	// n초 후에 이미 버튼을 놓은 상태라면 즉시 투척 실행
	if (bIsReleasedButton)
	{
		PerformThrowing(Holder);
	}
}

void APEWeaponGranade::PerformThrowing(AActor* Holder)
{
	if (bIsThrowingReady && bIsReleasedButton)
	{
		FPEAttackStats AttackStats;
		AttackStats.AttackRange = WeaponStats.Range;
		AttackStats.DamageAmount = WeaponStats.Damage;
		AttackStats.SpreadAngle = WeaponStats.Spread;
		AttackStats.ProjectileClass = WeaponStats.ProjectileClass;
		AttackStats.ProjectileSpeed = WeaponStats.ProjectileSpeed;
		AttackStats.ExplosionRadius = WeaponStats.ExplosionRadius;
		AttackStats.ExplosionDelay = WeaponStats.ExplosionDelayTime;
		AttackStats.CollisionChannel = ECC_Visibility; //TODO: Collision 수정 필요

		AttackComponent->ExcuteAttack(AttackStats);
		
		if (APEHero* PEHero = Cast<APEHero>(WeaponOwnerActor))
		{
			if (WeaponStats.IsAutomatic)
			{
				float ShotIntervalSeconds = 60.0f / WeaponStats.FireRate;
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
		CurrentAmmoCount--;
		BroadcastWeaponStateChanged();
	}
	bIsThrowingReady = false;
	bIsReleasedButton = false;
}
