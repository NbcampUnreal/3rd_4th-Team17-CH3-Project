// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Hero/Components/PEInventoryManagerComponent.h"

#include "Core/PEGameplayTags.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEStorableItemComponent.h"

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
	
	if (InventoryItems.Num() > MaxInventorySize) 
	{
		UE_LOG(LogPE, Warning, TEXT("Inventory is full!"));
		return;
	}

	FGameplayTag ItemTag;
	if (Item)
	{
		ItemTag = Item->GetItemTag();
	}
	
	// 동일한 아이템이 있는 경우 스택 추가
	if (UPEStorableItemComponent* ContainItem = GetItemByTag(ItemTag))
	{
		Item-> OnItemPickedUp(); 
		Item->DestoryItem(); //아이템이 주워졌을 때 이미 있는 아이템이면 제거
		ContainItem->AddItemCount(Item->GetItemCount());
		UpdateCurrentItemCount();
		UE_LOG(LogPE, Log, TEXT("Contained Item! Item count increased: %d"), ContainItem->GetItemCount());
	}
	else
	{
		InventoryItems.Add(ItemTag, Item);
		Item->OnItemPickedUp();
		UpdateCurrentItemCount();
		UE_LOG(LogPE, Log, TEXT("New item added to Inventory"));
	}

	// 인벤토리에 있는 모든 아이템을 로그에 출력 (테스트용 코드, UI 연결 시 삭제)
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
	// 테스트 코드 끝
}

void UPEInventoryManagerComponent::RemoveItemFromInventory(int32 Count, UPEStorableItemComponent* Item)
{
	FGameplayTag ItemTag = Item->GetItemTag();
	
	if (UPEStorableItemComponent* ContainItem = GetItemByTag(ItemTag))
	{
		//Todo: 아이템 개수가 Count보다 많을 경우 아이템 개수를 감소 시키고
		//       아이템을 복제해서 바닥에 떨어뜨리는 로직을 추가
		const FVector	&Location = Item->GetOwner()->GetActorLocation();
		const FRotator	&Rotation = Item->GetOwner()->GetActorRotation();
		ContainItem->OnItemDropped(Count, Location, Rotation);
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("Item not found in inventory: %s"), *Item->GetOwner()->GetName());
		return;
	}
}

bool UPEInventoryManagerComponent::IsItemInInventory(UPEStorableItemComponent* Item) const
{
	return true;
}

bool UPEInventoryManagerComponent::IsItemInInventoryByTag(const FGameplayTag &Tag) const
{
	for (const auto &Item: InventoryItems)
	{
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
	//Todo: 마지막 아이템이 버려질 때, 아이템이 주울 수 있는 상태로 바뀌어야 함
	//		RemoveItemFromInventory과 병합 필요 (해당 함수가 미완성)
	if (InventoryItems.Num() == 0) return;
	UPEStorableItemComponent* DropItem = InventoryItems.begin()->Value;
	if (DropItem)
	{
		if (DropItem->GetItemCount() == 1)
		{
			InventoryItems.Remove(DropItem->GetItemTag());
		}
		DropItem->ReduceItemCount(1,
			GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
		
	}
	else
	{
		UE_LOG(LogPE, Log, TEXT("Item not found"));
	}
}
