// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/Components/PEQuickSlotManagerComponent.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Characters/Hero/Interface/PEQuickSlotHandler.h"
#include "Core/PELogChannels.h"
#include "Items/Interface/PEQuickSlotItem.h"
#include "Items/Weapons/PEWeaponBase.h"
#include "Characters/Hero/PEHero.h"

/*
 * 퀵슬롯에서 필요한 기능
 * 1. 퀵슬롯 아이템 등록
 * 2. 퀵슬롯 아이템 해제
 * 3. 키 입력시 선택된 아이템을 리턴 (리턴된 값을 캐릭터에서 사용)
 * 4. 퀵슬롯 아이템 초기화
 */

UPEQuickSlotManagerComponent::UPEQuickSlotManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPEQuickSlotManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	EquipOwner = GetOwner();
}

void UPEQuickSlotManagerComponent::SetQuickSlotItem(EPEEquipmentType EquipmentType, AActor* ItemActor)
{
	if (!ItemActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetQuickSlotItem: ItemActor is null!"));
		return;
	}
	
	// 아이템을 이미 퀵슬롯에 등록한 경우, 기존 아이템을 해제
	if (QuickSlotItems.Contains(EquipmentType))
	{
		if (QuickSlotItems[EquipmentType])
		{
			if (IPEQuickSlotItem* QuickSlotItemInterface = Cast<IPEQuickSlotItem>(QuickSlotItems[EquipmentType]))
			{
				if (UPEQuickSlotItemComponent* QuickSlotItemComponent = QuickSlotItemInterface->GetQuickSlotItemComponent())
				{
					const FVector& Location = EquipOwner->GetActorLocation();
					const FRotator& Rotation = EquipOwner->GetActorRotation();
					
					QuickSlotItemComponent->OnItemDropped(Location, Rotation);
				}
			}
		}
		QuickSlotItems[EquipmentType] = ItemActor;
		UE_LOG(LogTemp, Log, TEXT("Updated quick slot item: %s for type: %d"), *ItemActor->GetName(), static_cast<int32>(EquipmentType));
	}
	else
	{
		QuickSlotItems.Add(EquipmentType, ItemActor);
		UE_LOG(LogTemp, Log, TEXT("Added quick slot item: %s for type: %d"), *ItemActor->GetName(), static_cast<int32>(EquipmentType));
	}

	if (IPEQuickSlotItem* QuickSlotItemInterface = Cast<IPEQuickSlotItem>(ItemActor))
	{
		if (UPEQuickSlotItemComponent* QuickSlotItemComponent = QuickSlotItemInterface->GetQuickSlotItemComponent())
		{
			QuickSlotItemComponent->OnItemPickedUp();
		}
	}

	UpdateQuickSlotInfoAndBroadcast();
}

AActor* UPEQuickSlotManagerComponent::SelectEquipment(EPEEquipmentType EquipmentType)
{
	if (IPEQuickSlotHandler* QuickSlotInterface = Cast<IPEQuickSlotHandler>(EquipOwner))
	{
		if (QuickSlotItems.Contains(EquipmentType) && QuickSlotItems[EquipmentType] != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Selected equipment: %s"), *QuickSlotItems[EquipmentType]->GetName());
			return QuickSlotItems[EquipmentType];
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PEQuickSlotManagerComponent: Owner %s does not implement IPEQuickSlot interface!"), *EquipOwner->GetName());
	}
	return nullptr;
}

void UPEQuickSlotManagerComponent::RemoveQuickSlotItem(EPEEquipmentType EquipmentType)
{
	if (QuickSlotItems.Contains(EquipmentType))
	{
		QuickSlotItems[EquipmentType] = nullptr;
		
		UpdateQuickSlotInfoAndBroadcast();
	}
}

void UPEQuickSlotManagerComponent::ClearQuickSlots()
{
	QuickSlotItems.Empty();
	
	UpdateQuickSlotInfoAndBroadcast();
}

bool UPEQuickSlotManagerComponent::ContainWeaponType(EPEEquipmentType EquipmentType)
{
	if (QuickSlotItems.Contains(EquipmentType) && QuickSlotItems[EquipmentType] != nullptr)
	{
		return true;
	}
	return false;
}

void UPEQuickSlotManagerComponent::DropHandEquipmentToWorld(EPEEquipmentType EquipmentType, const FVector& Location, const FRotator& Rotation)
{
	if (QuickSlotItems.Contains(EquipmentType))
	{
		if (IPEQuickSlotItem* QuickSlotItem = Cast<IPEQuickSlotItem>(QuickSlotItems[EquipmentType]))
		{
			QuickSlotItem->OnDropped(Location, Rotation);
			RemoveQuickSlotItem(EquipmentType);
			
			UE_LOG(LogPE, Log, TEXT("DropHandEquipmentToWorld: Dropped %d at Location: %s, Rotation: %s"),
				static_cast<int8>(EquipmentType), *Location.ToString(), *Rotation.ToString());
		}
		else
		{
			UE_LOG(LogPE, Warning, TEXT("DropHandEquipmentToWorld: QuickSlotItemComponent not found for EquipmentType %d!"), static_cast<int32>(EquipmentType));
		}
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("DropHandEquipmentToWorld: EquipmentType %d not found in QuickSlotItems!"), static_cast<int32>(EquipmentType));
	}
}

AActor* UPEQuickSlotManagerComponent::GetActorFromQuickSlot(EPEEquipmentType EquipmentType) const
{
	return QuickSlotItems.Contains(EquipmentType) ? QuickSlotItems[EquipmentType] : nullptr;
}

TMap<EPEEquipmentType, TObjectPtr<AActor>> UPEQuickSlotManagerComponent::GetQuickSlotItem(
	EPEEquipmentType EquipmentType) const
{
	return QuickSlotItems;
}

FInventoryInfo UPEQuickSlotManagerComponent::ConvertToQuickSlotInfo()
{
	FInventoryInfo QuickSlotInfo;
	const FPEGameplayTags& GameplayTags = FPEGameplayTags::Get();
	
	if (QuickSlotItems.Contains(EPEEquipmentType::Primary) && QuickSlotItems[EPEEquipmentType::Primary])
	{
		if (APEWeaponBase* WeaponBase = Cast<APEWeaponBase>(QuickSlotItems[EPEEquipmentType::Primary]))
		{
			const FPEWeaponData& WeaponStats = WeaponBase->GetWeaponStats();
			
			QuickSlotInfo.MainWeapon.ItemTexture = WeaponStats.IconTexture2D; 
			QuickSlotInfo.MainWeapon.ItemDescription = FText::FromString(WeaponStats.Description);
			QuickSlotInfo.MainWeapon.ItemTag = GameplayTags.Item_Weapon_RangeWeapon_MainWeapon;
			QuickSlotInfo.MainWeapon.CurrentAmmo = WeaponBase->GetCurrentAmmoCount();
			QuickSlotInfo.MainWeapon.TotalAmmo = WeaponStats.MaxAmmo;
			UE_LOG(LogPE, Log, TEXT("Converted Weapon to MainWeapon in QuickSlotInfo"));
		}
	}

	if (QuickSlotItems.Contains(EPEEquipmentType::Secondary) && QuickSlotItems[EPEEquipmentType::Secondary])
	{
		if (APEWeaponBase* WeaponBase = Cast<APEWeaponBase>(QuickSlotItems[EPEEquipmentType::Secondary]))
		{
			const FPEWeaponData& WeaponStats = WeaponBase->GetWeaponStats();
			
			QuickSlotInfo.SubWeapon.ItemTexture = WeaponStats.IconTexture2D;
			QuickSlotInfo.SubWeapon.ItemDescription = FText::FromString(WeaponStats.Description);
			QuickSlotInfo.SubWeapon.ItemTag = GameplayTags.Item_Weapon_RangeWeapon_SubWeapon;
			QuickSlotInfo.SubWeapon.CurrentAmmo = WeaponBase->GetCurrentAmmoCount();
			QuickSlotInfo.SubWeapon.TotalAmmo = WeaponStats.MaxAmmo;
			UE_LOG(LogPE, Log, TEXT("Converted Weapon to SubWeapon in QuickSlotInfo"));
		}
		
	}

	if (QuickSlotItems.Contains(EPEEquipmentType::Melee) && QuickSlotItems[EPEEquipmentType::Melee])
	{
		if (APEWeaponBase* WeaponBase = Cast<APEWeaponBase>(QuickSlotItems[EPEEquipmentType::Melee]))
		{
			const FPEWeaponData& WeaponStats = WeaponBase->GetWeaponStats();
			
			QuickSlotInfo.MeleeWeapon.ItemTexture = WeaponStats.IconTexture2D; 
			QuickSlotInfo.MeleeWeapon.ItemDescription = FText::FromString(WeaponStats.Description);
			QuickSlotInfo.MeleeWeapon.ItemTag = GameplayTags.Item_Weapon_MeleeWeapon;
			UE_LOG(LogPE, Log, TEXT("Converted Weapon to Melee in QuickSlotInfo"));
		}
	}
	
	if (QuickSlotItems.Contains(EPEEquipmentType::Healing) && QuickSlotItems[EPEEquipmentType::Healing])
	{
		if (APEWeaponBase* WeaponBase = Cast<APEWeaponBase>(QuickSlotItems[EPEEquipmentType::Healing]))
		{
			const FPEWeaponData& WeaponStats = WeaponBase->GetWeaponStats();
			
			QuickSlotInfo.QuickHeal.ItemTexture = WeaponStats.IconTexture2D; 
			QuickSlotInfo.QuickHeal.ItemDescription = FText::FromString(WeaponStats.Description);
			QuickSlotInfo.QuickHeal.ItemTag = GameplayTags.Item_Things_Heal;
			UE_LOG(LogPE, Log, TEXT("Converted Weapon to Healing in QuickSlotInfo"));
		}
	}
	
	if (QuickSlotItems.Contains(EPEEquipmentType::Throwable) && QuickSlotItems[EPEEquipmentType::Throwable])
	{
		if (APEWeaponBase* WeaponBase = Cast<APEWeaponBase>(QuickSlotItems[EPEEquipmentType::Throwable]))
		{
			const FPEWeaponData& WeaponStats = WeaponBase->GetWeaponStats();
			
			QuickSlotInfo.QuickGrenade.ItemTexture = WeaponStats.IconTexture2D; 
			QuickSlotInfo.QuickGrenade.ItemDescription = FText::FromString(WeaponStats.Description);
			QuickSlotInfo.QuickGrenade.ItemTag = GameplayTags.Item_Things_Grenade;
			UE_LOG(LogPE, Log, TEXT("Converted Weapon to Throwable in QuickSlotInfo"));
		}
	}

	return QuickSlotInfo;
}

void UPEQuickSlotManagerComponent::UpdateQuickSlotInfoAndBroadcast()
{
	if (APEHero* Hero = Cast<APEHero>(EquipOwner))
	{
		Hero->SetQuickSlotInfo(ConvertToQuickSlotInfo());
		Hero->BroadcastInventoryChanged();
	}
}
