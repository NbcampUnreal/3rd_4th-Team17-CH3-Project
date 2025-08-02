// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Hero/Components/PEInventoryManagerComponent.h"

#include "Core/PELogChannels.h"
#include "Items/PEItemBase.h"
#include "Items/Components/PEStorableItemComponent.h"

UPEInventoryManagerComponent::UPEInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPEInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPEInventoryManagerComponent::AddItemToInventory(UPEStorableItemComponent* Item)
{
	if (InventoryItems.Num() > MaxInventorySize) 
	{
		UE_LOG(LogPE, Warning, TEXT("Inventory is full!"));
		return;
	}

	if (InventoryItems.Contains(Item))
	{
		// Todo: 동일한 아이템이 있는 경우 스택 추가
		// AddItemStack();
		return;		
	}

	InventoryItems.Add(Item);
	UE_LOG(LogPE, Log, TEXT("Item added to Inventory"));

	// 인벤토리에 있는 모든 아이템을 로그에 출력 (테스트용 코드)
	UE_LOG(LogPE, Log, TEXT("Current Inventory Items (%d/%d):"), InventoryItems.Num(), MaxInventorySize);
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i])
		{
			UE_LOG(LogPE, Log, TEXT("  [%d] %s"), i, *InventoryItems[i]->GetName());
		}
		else
		{
			UE_LOG(LogPE, Log, TEXT("  [%d] NULL Item"), i);
		}
	}
	//
}

void UPEInventoryManagerComponent::RemoveItemFromInventory(UPEStorableItemComponent* Item)
{
	
}

bool UPEInventoryManagerComponent::IsItemInInventory(UPEStorableItemComponent* Item) const
{
	return true;
}

void UPEInventoryManagerComponent::ClearInventory()
{
	InventoryItems.Empty();
	UE_LOG(LogPE, Log, TEXT("Inventory cleared"));
}

UPEStorableItemComponent* UPEInventoryManagerComponent::GetItemByTag(const FPEGameplayTags& Tag)
{
	//Todo: Tag를 사용하여 아이템 검색 구현
	return nullptr;
}

TArray<TObjectPtr<UPEStorableItemComponent>> UPEInventoryManagerComponent::GetInventoryItems() const
{
	return InventoryItems;
}

void UPEInventoryManagerComponent::SortInventory()
{
	//Todo: 인벤토리 정렬 구현
}
