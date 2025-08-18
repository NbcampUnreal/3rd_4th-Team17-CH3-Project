// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEAttackHitscanComponent.h"
#include "Combat/Components/PEReceiveAttackComponent.h"
#include "Engine/World.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Core/PELogChannels.h"
#include "DrawDebugHelpers.h"
#include "Items/Weapons/PEWeaponBase.h"

UPEAttackHitscanComponent::UPEAttackHitscanComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPEAttackHitscanComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPEAttackHitscanComponent::PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction)
{
	
	// 히트스캔 공격 구현
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	UE_LOG(LogPE, Log, TEXT("HitScan Attack Performed!"));
	
	// LineTrace 설정
	FVector EndLocation = StartLocation + (Direction * AttackStats.AttackRange);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); // 자기 자신은 무시
	
	// LineTrace 실행
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		AttackStats.HitscanChannel, 
		QueryParams
	);
	
	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		
		// 맞은 컴포넌트 자체가 PEReceiveAttackComponent인지 확인
		if (UPEReceiveAttackComponent* ReceiveAttackComponent = Cast<UPEReceiveAttackComponent>(HitComponent))
		{
			UE_LOG(LogPE, Log, TEXT("Hit PEReceiveAttackComponent: %s on Actor: %s"), 
				*ReceiveAttackComponent->GetName(), *HitActor->GetName());
			
			AActor* InstigatorActor = nullptr;
			if (APEWeaponBase* Weapon = Cast<APEWeaponBase>(GetOwner()))
			{
				InstigatorActor = Weapon->GetItemOwner();
			}
			else
			{
				InstigatorActor = GetOwner();
			}
			ReceiveAttackComponent->ReceiveDamage(
				AttackStats.DamageAmount,
				HitResult.Location,
				HitResult.Normal,
				InstigatorActor
			);
		}
	}
	
#ifdef WITH_EDITOR
	// 디버그용 ray 그리기
	FColor DebugColor = bHit ? FColor::Red : FColor::Green;  // 히트하면 빨간색, 안 하면 초록색
	FVector DebugEndLocation = bHit ? HitResult.Location : EndLocation;
	DrawDebugLine(World, StartLocation, DebugEndLocation, DebugColor, false, 2.0f, 0, 2.0f);
	
	// 히트 지점에 디버그 구체 그리기
	if (bHit)
	{
		DrawDebugSphere(World, HitResult.Location, 5.0f, 12, FColor::Yellow, false, 2.0f);
	}
#endif // WITH_EDITOR
}
