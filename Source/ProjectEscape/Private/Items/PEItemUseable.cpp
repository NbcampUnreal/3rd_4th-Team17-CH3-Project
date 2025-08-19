// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemUseable.h"

#include "Characters/Hero/PEHero.h"
#include "Characters/Hero/Components/PEInventoryManagerComponent.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Items/Components/PEUseableComponent.h"
#include "Player/PEPlayerState.h"


class APEPlayerState;

APEItemUseable::APEItemUseable()
{
	QuickSlotItemComponent = CreateDefaultSubobject<UPEQuickSlotItemComponent>(TEXT("QuickSlotItemComponent"));
	UseableComponent = CreateDefaultSubobject<UPEUseableComponent>(TEXT("UseableComponent"));
}

void APEItemUseable::BeginPlay()
{
	Super::BeginPlay();
	
	// 데이터 테이블에서 데이터 로드
	if (EquipmentDataTable && !EquipmentRowName.IsNone())
	{
		if (FPEWeaponData* EquipmentData = EquipmentDataTable->FindRow<FPEWeaponData>(EquipmentRowName, TEXT("")))
		{
			EquipmentStats = *EquipmentData;
		}
	}
}

void APEItemUseable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FPEWeaponData APEItemUseable::GetEquipmentStats()
{
	return EquipmentStats;
}

AActor* APEItemUseable::GetItemOwner() const
{
	return ItemOwnerActor;
}

void APEItemUseable::OnDropped(const FVector& Location, const FRotator& Rotation)
{
	BroadcastEmptyHUDState();
	
	ItemOwnerActor = nullptr;
	OnRelease();
	
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	UE_LOG(LogPE, Log, TEXT("APEItemHealing::OnDropped called on"));	
}

UPEQuickSlotItemComponent* APEItemUseable::GetQuickSlotItemComponent() const
{
	return QuickSlotItemComponent;
}

EPEEquipmentType APEItemUseable::GetEquipmentType() const
{
	return EquipmentStats.Type;
}

void APEItemUseable::DoPrimaryAction(AActor* Holder)
{
	// NOTE: 실제 구현은 자식 클래스에 작성해야 합니다.
}

void APEItemUseable::CompletePrimaryAction(AActor* Holder)
{
	// NOTE: 실제 구현은 자식 클래스에 작성해야 합니다.
}

void APEItemUseable::DoSecondaryAction(AActor* Holder)
{
	// NOTE: 실제 구현은 자식 클래스에 작성해야 합니다.
}

void APEItemUseable::DoTertiaryAction(AActor* Holder)
{
	// NOTE: 실제 구현은 자식 클래스에 작성해야 합니다.
}

void APEItemUseable::OnHand(AActor* NewOwner)
{
	AttachToOwner();
	
	BroadcastHUDStateChanged();
}

void APEItemUseable::OnRelease()
{
	DetachFromOwner();
	
	// 무기를 놓으면 맨손 상태가 되므로 빈 FPEEquipmentInfo를 브로드캐스트
	BroadcastEmptyHUDState();
}

UPEUseableComponent* APEItemUseable::GetUseableComponent() const
{
	return UseableComponent;	
}

FGameplayTag APEItemUseable::GetItemTag() const
{
	return ItemStats.ItemTag;
}

FPEEquipmentInfo APEItemUseable::CreateCurrentEquipmentInfo() const
{
	FPEEquipmentInfo EquipmentInfo;
	//TODO: Weapon Data Table을 재활용
	EquipmentInfo.EquipmentName = EquipmentStats.Name;
	EquipmentInfo.AmmoCount = FString::Printf(TEXT(" "));
	EquipmentInfo.EquipmentIcon = EquipmentStats.IconTexture2D;
	EquipmentInfo.EquipmentDescription = FString::Printf(TEXT("Damage: %d, Range: %.1f"), 
		EquipmentStats.Damage, EquipmentStats.Range);

	return EquipmentInfo;
}

void APEItemUseable::BroadcastHUDStateChanged()
{
	FPEEquipmentInfo EquipmentInfo = CreateCurrentEquipmentInfo();

	if (APEHero* Hero = Cast<APEHero>(ItemOwnerActor))
	{
		Hero->BroadCastEquipmentChanged(EquipmentInfo);
	
		// 델리게이트 브로드캐스트 정보 로그 출력 (테스트 용 코드)
		UE_LOG(LogPE, Log, TEXT("Broadcasting weapon state changed - Name: %s, Count: %s, Description: %s"), 
			*EquipmentInfo.EquipmentName.ToString(),
			*EquipmentInfo.AmmoCount,
			*EquipmentInfo.EquipmentDescription);
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("BroadcastWeaponStateChanged: WeaponOwnerActor is not APEHero"));
	}
}

void APEItemUseable::BroadcastEmptyHUDState()
{
	FPEEquipmentInfo EmptyEquipmentInfo;
	EmptyEquipmentInfo.EquipmentName = FName(" ");
	EmptyEquipmentInfo.AmmoCount = TEXT(" ");
	EmptyEquipmentInfo.EquipmentDescription = TEXT(" ");
	EmptyEquipmentInfo.EquipmentIcon = nullptr;
	
	if (APEHero* Hero = Cast<APEHero>(ItemOwnerActor))
	{
		Hero->BroadCastEquipmentChanged(EmptyEquipmentInfo);
	
		// 델리게이트 브로드캐스트 정보 로그 출력 (테스트 용 코드)
		UE_LOG(LogPE, Log, TEXT("Broadcasting weapon state changed - Name: %s, Count: %s, Description: %s"), 
			*EmptyEquipmentInfo.EquipmentName.ToString(),
			*EmptyEquipmentInfo.AmmoCount,
			*EmptyEquipmentInfo.EquipmentDescription);
		UE_LOG(LogPE, Log, TEXT("Broadcasting weapon state changed - Empty Weapon State"));
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("BroadcastWeaponStateChanged: WeaponOwnerActor is not APEHero"));
	}
}

void APEItemUseable::AttachToOwner()
{
	// Remove existing weapon Actor
	if (AttachedActor)
	{
		DetachFromOwner();
	}

	if (EquipmentStats.ActorToAttach)
	{
		if (IPEWeaponAttachable* AttachParent = Cast<IPEWeaponAttachable>(ItemOwnerActor))
		{
			if (UWorld* World = GetWorld())
			{
				FActorSpawnParameters Params;
				AttachedActor = World->SpawnActor<AActor>(EquipmentStats.ActorToAttach, Params);
				AttachParent->AttachWeapon(AttachedActor, EquipmentStats.AttachTransform);
			}
		}
	}
}

void APEItemUseable::DetachFromOwner()
{
	if (AttachedActor)
	{
		FDetachmentTransformRules Rule = FDetachmentTransformRules::KeepWorldTransform;
		AttachedActor->DetachFromActor(Rule);
		AttachedActor->Destroy();
		AttachedActor = nullptr;
	}
}