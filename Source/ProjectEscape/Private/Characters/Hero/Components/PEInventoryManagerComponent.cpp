// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Hero/Components/PEInventoryManagerComponent.h"
#include "Items/PEItemBase.h"

UPEInventoryManagerComponent::UPEInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPEInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPEInventoryManagerComponent::AddItemToInventory(APEItemBase* Item)
{
	if (InventoryItems.Num() > MaxInventorySize) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is full! Cannot add item: %s"), *Item->GetName());
		return;
	}

	if (InventoryItems.Contains(Item))
	{
		return;		
	}

	InventoryItems.Add(Item);
}

void UPEInventoryManagerComponent::RemoveItemFromInventory(class APEItemBase* Item)
{
}

bool UPEInventoryManagerComponent::IsItemInInventory(class APEItemBase* Item) const
{
	return true;
}

void UPEInventoryManagerComponent::ClearInventory()
{
	InventoryItems.Empty();
	UE_LOG(LogTemp, Log, TEXT("Inventory cleared"));
}

TArray<TObjectPtr<class APEItemBase>> UPEInventoryManagerComponent::GetInventoryItems() const
{
	return InventoryItems;
}

