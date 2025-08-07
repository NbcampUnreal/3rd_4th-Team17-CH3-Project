// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PEWeaponBase.h"

#include "Items/PEEquipmentType.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Items/Components/PEUseableComponent.h"
#include "Items/Components/PEInteractableComponent.h"

APEWeaponBase::APEWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 상호작용 컴포넌트 생성 및 설정
	InteractableComponent = CreateDefaultSubobject<UPEInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
	InteractableComponent->SetHiddenInGame(false);

	// 장착 컴포넌트 생성 및 설정
	HoldableComponent = CreateDefaultSubobject<UPEUseableComponent>(TEXT("HoldableComponent"));

	// 퀵슬롯 아이템 컴포넌트 생성 및 설정
	QuickSlotItemComponent = CreateDefaultSubobject<UPEQuickSlotItemComponent>(TEXT("QuickSlotItemComponent"));

	bIsInHand = false;
	LastAttackTime = 0.0f;
}

void APEWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// AttackComponent 생성
	if (!AttackComponent)
	{
		AttackComponent = CreateAttackComponent();
		if (AttackComponent)
		{
			AttackComponent->RegisterComponent();
			UE_LOG(LogPE, Log, TEXT("AttackComponent created and registered for %s"), *GetName());
		}
		else
		{
			UE_LOG(LogPE, Error, TEXT("Failed to create AttackComponent for %s"), *GetName());
		}
	}

	// 데이터 테이블에서 무기 데이터 로드
	if (WeaponDataTable && !WeaponRowName.IsNone())
	{
		if (FPEWeaponData* WeaponData = WeaponDataTable->FindRow<FPEWeaponData>(WeaponRowName, TEXT("")))
		{
			WeaponStats = *WeaponData;
		}
	}
}

void APEWeaponBase::Interact(AActor* Interactor)
{
	UE_LOG(LogPE, Warning, TEXT("Interact called on %s by %s"), *GetName(), *Interactor->GetName());
	
	WeaponOwnerActor = Interactor;
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

void APEWeaponBase::Use(AActor* Holder)
{
	UE_LOG(LogTemp, Warning, TEXT("Use called on %s by %s"), *GetName(), *Holder->GetName());
	
	// FireRate 체크 (RPM을 초당 발사 횟수로 변환)
	if (WeaponStats.FireRate > 0.0f)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		float TimeBetweenShots = 60.0f / WeaponStats.FireRate; // RPM을 초 단위 간격으로 변환
		
		if (CurrentTime - LastAttackTime < TimeBetweenShots)
		{
			// 아직 발사할 수 없음
			UE_LOG(LogTemp, Log, TEXT("Attack blocked by FireRate. Time since last attack: %f, Required interval: %f"), 
				CurrentTime - LastAttackTime, TimeBetweenShots);
			return;
		}
		
		LastAttackTime = CurrentTime;
	}
	
	FPEAttackStats AttackStats;
	AttackStats.AttackRange = WeaponStats.Range;
	AttackStats.DamageAmount = WeaponStats.Damage;
	AttackStats.CollisionChannel = ECC_Visibility;

	AttackComponent->ExcuteAttack(AttackStats);
}

void APEWeaponBase::OnHand(AActor* NewOwner)
{
	bIsInHand = true;
}

bool APEWeaponBase::IsInteractable() const
{
	if (WeaponOwnerActor) // 현재 주인이 있으면 상호작용 불가
	{
		return false;
	}
	return true;
}

UPEUseableComponent* APEWeaponBase::GetUseableComponent() const
{
	return HoldableComponent;
}

AActor* APEWeaponBase::GetItemOwner() const
{
	return WeaponOwnerActor;
}

void APEWeaponBase::OnDropped()
{
	WeaponOwnerActor = nullptr;
}

UPEQuickSlotItemComponent* APEWeaponBase::GetQuickSlotItemComponent() const
{
	return QuickSlotItemComponent;
}

EPEEquipmentType APEWeaponBase::GetEquipmentType() const
{
	return WeaponStats.Type;
}

UPEAttackBaseComponent* APEWeaponBase::CreateAttackComponent()
{
	UE_LOG(LogPE, Warning, TEXT("APEWeaponBase::CreateAttackComponent() called"));
    return NewObject<UPEAttackBaseComponent>(this);
}
