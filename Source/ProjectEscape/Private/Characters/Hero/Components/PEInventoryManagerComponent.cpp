// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Hero/Components/PEInventoryManagerComponent.h"

#include "Core/PEGameplayTags.h"
#include "Core/PELogChannels.h"
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

	FGameplayTag ItemTag = Item->GetItemTag();
	
	// 동일한 아이템이 있는 경우 스택 추가
	if (UPEStorableItemComponent* ContainItem = GetItemByTag(Item->GetItemTag()))
	{
		Item-> OnItemPickedUp(); //Todo: 아이템이 주워졌을 때 이미 있는 아이템이면 제거하도록 변경
		ContainItem->AddItemCount(Item->GetItemCount());
		UE_LOG(LogPE, Log, TEXT("Contain Item! Item count increased: %d"), ContainItem->GetItemCount());
		return;		
	}

	InventoryItems.Add(ItemTag, Item);
	Item->OnItemPickedUp();
	UE_LOG(LogPE, Log, TEXT("Item added to Inventory"));

	// 인벤토리에 있는 모든 아이템을 로그에 출력 (테스트용 코드)
	UE_LOG(LogPE, Log, TEXT("Current Inventory Items (%d/%d):"), InventoryItems.Num(), MaxInventorySize);
	for (const auto &ItemPair : InventoryItems)
	{
		if (ItemPair.Value)
		{
			UE_LOG(LogPE, Log, TEXT(" - Item: %s"), *ItemPair.Value->GetOwner()->GetName());
		}
		else
		{
			UE_LOG(LogPE, Warning, TEXT(" - Item is null!"));
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

bool UPEInventoryManagerComponent::IsItemInInventoryByTag(const FGameplayTag &Tag) const
{
	for (const auto &Item: InventoryItems)
	{
		//Todo: Tag를 사용하여 아이템 검색 구현
		if (Item.Value && Item.Value->GetItemTag() == Tag)
		{
			return true;
		}
	}
	return false;
}

void UPEInventoryManagerComponent::ClearInventory()
{
	InventoryItems.Empty();
	UE_LOG(LogPE, Log, TEXT("Inventory cleared"));
}

UPEStorableItemComponent* UPEInventoryManagerComponent::GetItemByTag(const FGameplayTag& Tag)
{
	for (const auto &Item: InventoryItems)
	{
		//Todo: Tag를 사용하여 아이템 검색 구현
		if (Item.Value && Item.Value->GetItemTag() == Tag)
		{
			return Item.Value;
		}
	}
	UE_LOG(LogPE, Log, TEXT("Item not found"));
	return nullptr;
}

void UPEInventoryManagerComponent::SortInventory()
{
	//Todo: 인벤토리 정렬 구현
}
