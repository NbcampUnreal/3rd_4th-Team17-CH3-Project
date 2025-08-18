// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Projectile/PEProjectileExplosive.h"

#include "Kismet/GameplayStatics.h"


APEProjectileExplosive::APEProjectileExplosive()
{
}

void APEProjectileExplosive::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void APEProjectileExplosive::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	PerformExplosion();
}

void APEProjectileExplosive::PerformExplosion()
{
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		ProjectileStats.DamageAmount,
		GetActorLocation(),
		ProjectileStats.ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // 무시할 액터들
		this,              // 데미지 원인자
		GetInstigatorController(),
		true,              // 풀 데미지 적용
		ProjectileStats.HitscanChannel
	);
	
	if (HitEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitEffect,
			GetActorLocation()
		);
	}

	// 사운드 이펙트
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HitSound,
			GetActorLocation()
		);
	}
	OnProjectileExpired();
}
