// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Projectile/PEProjectileDelayedExplosive.h"

#include "Core/PELogChannels.h"


APEProjectileDelayedExplosive::APEProjectileDelayedExplosive()
{
	
}

void APEProjectileDelayedExplosive::BeginPlay()
{
	Super::BeginPlay();

}

void APEProjectileDelayedExplosive::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ExplosionTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);
		ExplosionTimerHandle.Invalidate();
	};
}

void APEProjectileDelayedExplosive::Launch(AActor* InInstigator, const FPEAttackStats& AttackStats,
                                           const FVector& StartLocation, const FVector& Direction)
{
	Super::Launch(InInstigator, AttackStats, StartLocation, Direction);

	UE_LOG(LogPE, Log, TEXT("APEProjectileDelayedExplosive::Launch called with AttackStats: %f"), AttackStats.ExplosionDelay);
	// 지연 폭발 타이머 설정
	GetWorldTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&APEProjectileDelayedExplosive::PerformExplosion,
		AttackStats.ExplosionDelay,
		false
	);
}

void APEProjectileDelayedExplosive::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// 지연신관은 부딪혀도 폭발하지 않음
}




