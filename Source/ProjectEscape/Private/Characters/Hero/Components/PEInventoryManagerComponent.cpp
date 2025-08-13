// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Hero/Components/PEInventoryManagerComponent.h"

#include "NativeGameplayTags.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEStorableItemComponent.h"
#include "Characters/Hero/PEHero.h"
#include "UI/Inventory/PEInventoryType.h"

UPEInventoryManagerComponent::UPEInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	MaxInventorySize = 10;
	CurrentItemInInventroyCount = 0;
}

void UPEInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPEInventoryManagerComponent::AddItemToInventory(UPEStorableItemComponent* Item)
{
	if (!Item)
    {
        UE_LOG(LogPE, Warning, TEXT("AddItemToInventory: Item is null!"));
        return;
    }

	/*
	 *	TODO: 아이템 습득 시 추가 예외처리 필요
	 *	1. 새로운 아이템을 습득한 경우
	 *	2. 이미 인벤토리에 있는 아이템을 습득한 경우
	 *	3. 인벤토리가 가득 찬 경우
	 *	4. 이미 인벤토리에 있는 아이템이며, 기존 아이템에 스택은 가능하지만, 스택을 더 늘릴 수 없는 경우
	 *		(e.g. 이미 인벤토리에 10칸 차지,
	 *		아이템 최대 스택이 10인 아이템 A를 9개 갖고 있고, 재차 습득한 아이템 A의 개수가 2개 이상인 경우)
	 *	5.
	 */
	if (CurrentItemInInventroyCount > MaxInventorySize)
	{
		UE_LOG(LogPE, Warning, TEXT("Inventory is full!"));
		return;
	}

	FGameplayTag ItemTag = Item->GetItemTag();

	if (CurrentItemInInventroyCount == MaxInventorySize)
	{
		if (UPEStorableItemComponent* ContainItem = GetItemByTag(ItemTag))
		{
			int32 CurrentNeedItemCount = (ContainItem->GetStackCount() * ContainItem->GetStackCapacity()) - ContainItem->GetItemCount();

			if (CurrentNeedItemCount > 0)
			{
				int32 NeedItemCount = FMath::Min(Item->GetItemCount(), CurrentNeedItemCount);

				ContainItem->AddItemCount(NeedItemCount);
				Item->ReduceItemCount(NeedItemCount);
				BroadcastInventoryChanged();
			}
		}
		return;
	}
	
	// 동일한 아이템이 있는 경우 스택 추가
	if (UPEStorableItemComponent* ContainItem = GetItemByTag(ItemTag))
	{
		Item-> OnItemPickedUp();
		Item->DestroyItem(); //아이템이 주워졌을 때 이미 있는 아이템이면 제거
		ContainItem->AddItemCount(Item->GetItemCount());
		UpdateCurrentItemCount();
		BroadcastInventoryChanged(); 
		UE_LOG(LogPE, Log, TEXT("Contained Item! Item count increased: %d"), ContainItem->GetItemCount());
	}
	else
	{
		InventoryItems.Add(ItemTag, Item);
		Item->OnItemPickedUp();
		Item->SetInventroyManagerComponent(this);
		UpdateCurrentItemCount();
		BroadcastInventoryChanged(); 
		UE_LOG(LogPE, Log, TEXT("New item added to Inventory"));
	}

	// 인벤토리에 있는 모든 아이템을 로그에 출력 (테스트용 코드, UI 연결 시 삭제)
	UE_LOG(LogPE, Log, TEXT("Current Inventory Items (%d/%d):"), CurrentItemInInventroyCount, MaxInventorySize);
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
	// 테스트 코드 끝
}

void UPEInventoryManagerComponent::DropItemFromInventoryByTag(const int32& Count, const FGameplayTag& Tag)
{
	if (UPEStorableItemComponent* DropItem = GetItemByTag(Tag))
	{
		if (DropItem->GetItemCount() <= Count)
		{
			InventoryItems.Remove(DropItem->GetItemTag());
		}
		int32 DropCount = FMath::Clamp(Count, 0, DropItem->GetItemCount());
		DropItem->OnItemDropped(DropCount, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		BroadcastInventoryChanged(); 
	}
	else
	{
		UE_LOG(LogPE, Log, TEXT("Item not found"));
	}
}

void UPEInventoryManagerComponent::RemoveItemFromInventoryByTag(const FGameplayTag& Tag)
{
	if (InventoryItems.Contains(Tag))
	{
		InventoryItems[Tag] = nullptr; // 아이템 제거
		BroadcastInventoryChanged(); 
	}
}

void UPEInventoryManagerComponent::ClearInventory()
{
	InventoryItems.Empty();
	BroadcastInventoryChanged(); 
	UE_LOG(LogPE, Log, TEXT("Inventory cleared"));
}

UPEStorableItemComponent* UPEInventoryManagerComponent::GetItemByTag(const FGameplayTag& Tag) const
{
	if (InventoryItems.Contains(Tag))
	{
		return InventoryItems[Tag];
	}
	UE_LOG(LogPE, Warning, TEXT("Item with tag %s not found in inventory"), *Tag.ToString());
	return nullptr;
}

void UPEInventoryManagerComponent::SortInventory()
{
	//Todo: 인벤토리 정렬 구현
}

void UPEInventoryManagerComponent::UpdateCurrentItemCount()
{
	// 현재 아이템 개수는 스택 개수의 합으로 계산
	// Note: 인벤토리에 있는 아이템의 수가 적어 순회해서 계산하는데 더 나은 방법이 있을까요?
	int32 Count = 0;
	for (const auto &Item: InventoryItems)
	{
		if (Item.Value)
		{
			Count += Item.Value->GetStackCount();
		}
	}
	CurrentItemInInventroyCount = Count;
	UE_LOG(LogPE, Log, TEXT("Current item count in inventory: %d"), CurrentItemInInventroyCount);
}

void UPEInventoryManagerComponent::ItemDropTest()
{
	//NOTE: Test용 코드
	if (InventoryItems.Num() == 0)
	{
		UE_LOG(LogPE, Warning, TEXT("No items in inventory to drop!"));
		return;
	}
	FGameplayTag Tag = InventoryItems.begin()->Key;
	DropItemFromInventoryByTag(1, Tag);
}

FInventoryInfo UPEInventoryManagerComponent::ConvertToInventoryList() const
{
	FInventoryInfo InventoryInfo;
	FInventoryBagSlotInfo BagSlotInfo;

	for (const auto &Item: InventoryItems)
	{
		if (Item.Value)
		{
			int32 ItemCount = Item.Value->GetItemCount();
			int32 ItemCapacity = Item.Value->GetStackCapacity();
			while (ItemCount > 0)
			{
				FPEItemData ItemData = Item.Value->GetItemStats();
				BagSlotInfo.ItemTag = Item.Key;
				BagSlotInfo.ItemSprite = ItemData.IconSprite;
				BagSlotInfo.ItemTexture = ItemData.IconTexture;
				BagSlotInfo.ItemDescription = ItemData.Description; 

				// MaxStackCount와 IsStackable이 필요한 변수인지 확인해야 함
				BagSlotInfo.MaxStackCount = ItemData.StackCapacity;
				BagSlotInfo.IsStackable = false;
				
				if (ItemCount > ItemCapacity)
				{
					BagSlotInfo.StackCount = ItemCapacity;
					ItemCount -= ItemCapacity;
				}
				else
				{
					BagSlotInfo.StackCount = ItemCount;
					ItemCount = 0;
				}
				
				InventoryInfo.Bags.Add(BagSlotInfo);
			}
		}
		else
		{
			UE_LOG(LogPE, Warning, TEXT("ConvertToInventoryList: Item is null for tag %s"), *Item.Key.ToString());
		}
	}

	//TODO: 퀵슬롯 무기에 대한 연결 필요
	
	return InventoryInfo;
}

void UPEInventoryManagerComponent::BroadcastInventoryChanged()
{
	if (APEHero* Hero = Cast<APEHero>(GetOwner()))
	{
		FInventoryInfo CurrentInventoryList = ConvertToInventoryList();
		Hero->OnInventoryChanged.Broadcast(CurrentInventoryList);
		UE_LOG(LogPE, Log, TEXT("BroadcastInventoryChanged: Inventory change broadcasted to UI"));
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("BroadcastInventoryChanged: Owner is not APEHero"));
	}
}
