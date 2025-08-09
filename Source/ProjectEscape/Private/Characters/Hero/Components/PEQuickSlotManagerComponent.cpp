// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/Components/PEQuickSlotManagerComponent.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Characters/Hero/Interface/PEQuickSlotHandler.h"
#include "Core/PELogChannels.h"
#include "Items/Interface/PEQuickSlotItem.h"

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
}

void UPEQuickSlotManagerComponent::ClearQuickSlots()
{
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
			UE_LOG(LogPE, Log, TEXT("DropHandEquipmentToWorld: Dropped %s at Location: %s, Rotation: %s"),
				*QuickSlotItems[EquipmentType]->GetName(), *Location.ToString(), *Rotation.ToString());
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
