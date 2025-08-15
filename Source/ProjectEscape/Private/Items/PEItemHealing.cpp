// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemHealing.h"

#include "Characters/Hero/PEHero.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Items/Components/PEUseableComponent.h"


APEItemHealing::APEItemHealing()
{
	QuickSlotItemComponent = CreateDefaultSubobject<UPEQuickSlotItemComponent>(TEXT("QuickSlotItemComponent"));
	UseableComponent = CreateDefaultSubobject<UPEUseableComponent>(TEXT("UseableComponent"));
}

void APEItemHealing::BeginPlay()
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

FPEWeaponData APEItemHealing::GetEquipmentStats()
{
	return EquipmentStats;
}

AActor* APEItemHealing::GetItemOwner() const
{
	return ItemOwnerActor;
}

void APEItemHealing::OnDropped(const FVector& Location, const FRotator& Rotation)
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

UPEQuickSlotItemComponent* APEItemHealing::GetQuickSlotItemComponent() const
{
	return QuickSlotItemComponent;
}

EPEEquipmentType APEItemHealing::GetEquipmentType() const
{
	return EquipmentStats.Type;
}

void APEItemHealing::DoPrimaryAction(AActor* Holder)
{
	//Todo: Heal logic here
	UE_LOG(LogPE, Log, TEXT("APEItemHealing::DoPrimaryAction called"));
}

void APEItemHealing::CompletePrimaryAction(AActor* Holder)
{
}

void APEItemHealing::DoSecondaryAction(AActor* Holder)
{
}

void APEItemHealing::DoTertiaryAction(AActor* Holder)
{
}

void APEItemHealing::OnHand(AActor* NewOwner)
{
	AttachToOwner();

	BroadcastHUDStateChanged();
}

void APEItemHealing::OnRelease()
{

	DetachFromOwner();
	
	// 무기를 놓으면 맨손 상태가 되므로 빈 FPEEquipmentInfo를 브로드캐스트
	BroadcastEmptyHUDState();
}

UPEUseableComponent* APEItemHealing::GetUseableComponent() const
{
	return UseableComponent;	
}

FPEEquipmentInfo APEItemHealing::CreateCurrentEquipmentInfo() const
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

void APEItemHealing::BroadcastHUDStateChanged()
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

void APEItemHealing::BroadcastEmptyHUDState()
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

void APEItemHealing::AttachToOwner()
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

void APEItemHealing::DetachFromOwner()
{
	if (AttachedActor)
	{
		FDetachmentTransformRules Rule = FDetachmentTransformRules::KeepWorldTransform;
		AttachedActor->DetachFromActor(Rule);
		AttachedActor->Destroy();
		AttachedActor = nullptr;
	}
}



