// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEAttackProjectileComponent.h"
#include "Items/Weapons/Projectile/PEProjectileBase.h"
#include "Engine/World.h"


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
	if (!AttackStats.ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is not set in AttackStats"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null in PEAttackProjectileComponent::PerformAttack"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Attempting to spawn projectile: %s at location: %s"), 
		*AttackStats.ProjectileClass->GetName(), *StartLocation.ToString());

	// 투사체 스폰 파라미터 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 투사체 생성 (회전은 Launch에서 처리하므로 기본 회전 사용)
	APEProjectileBase* Projectile = World->SpawnActor<APEProjectileBase>(
		AttackStats.ProjectileClass,
		StartLocation,
		FRotator::ZeroRotator, // 회전은 Launch에서 설정됨
		SpawnParams
	);

	// 투사체가 성공적으로 생성되었다면 Launch 실행
	if (Projectile)
	{
		Projectile->Launch(AttackStats, StartLocation, Direction);
		UE_LOG(LogTemp, Log, TEXT("Projectile launched with physics-based movement"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile. ProjectileClass: %s, StartLocation: %s"), 
			*AttackStats.ProjectileClass->GetName(), *StartLocation.ToString());
	}
}
