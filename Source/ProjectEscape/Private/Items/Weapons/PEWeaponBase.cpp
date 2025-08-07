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
}

void APEWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

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
	// NOTE: 실제 사용 로직은 자식 클래스에서 구현해야 합니다.
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
	return EquipmentType;
}

UPEAttackBaseComponent* APEWeaponBase::CreateAttackComponent()
{
	UE_LOG(LogPE, Warning, TEXT("APEWeaponBase::CreateAttackComponent() called"));
    return NewObject<UPEAttackBaseComponent>(this);
}
