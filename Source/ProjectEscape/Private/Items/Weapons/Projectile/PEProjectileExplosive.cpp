// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Projectile/PEProjectileExplosive.h"

#include "Kismet/GameplayStatics.h"


APEProjectileExplosive::APEProjectileExplosive()
{
	
}

void APEProjectileExplosive::BeginPlay()
{
	Super::BeginPlay();
}

void APEProjectileExplosive::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	PerformExplosion();
}

void APEProjectileExplosive::PerformExplosion()
{
	float ExplosionRadius = 500.0f;
    
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		ProjectileStats.DamageAmount,
		GetActorLocation(),
		ExplosionRadius,
		UDamageType::StaticClass(),
		TArray<AActor*>(), // 무시할 액터들
		this,              // 데미지 원인자
		GetInstigatorController(),
		true,              // 풀 데미지 적용
		ProjectileStats.CollisionChannel
	);

	OnProjectileExpired();
}
