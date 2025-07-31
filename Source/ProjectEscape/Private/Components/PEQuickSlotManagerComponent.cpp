// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PEQuickSlotManagerComponent.h"

#include "Components/PEQuickSlotItemComponent.h"
#include "Interface/PEQuickSlot.h"
#include "Interface/PEQuickSlotItem.h"

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
	if (QuickSlotItems.Contains(EquipmentType))
	{
		if (QuickSlotItems[EquipmentType])
		{
			if (IPEQuickSlotItem* QuickSlotItemInterface = Cast<IPEQuickSlotItem>(QuickSlotItems[EquipmentType]))
			{
				UPEQuickSlotItemComponent* QuickSlotItemComponent = QuickSlotItemInterface->GetQuickSlotItemComponent();
				if (QuickSlotItemComponent)
				{
					QuickSlotItemComponent->OnItemDropped(EquipOwner);
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
}

AActor* UPEQuickSlotManagerComponent::SelectEquipment(EPEEquipmentType EquipmentType)
{
	if (IPEQuickSlot* QuickSlotInterface = Cast<IPEQuickSlot>(EquipOwner))
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

TMap<EPEEquipmentType, TObjectPtr<AActor>> UPEQuickSlotManagerComponent::GetQuickSlotItem(
	EPEEquipmentType EquipmentType) const
{
	return QuickSlotItems;
}
