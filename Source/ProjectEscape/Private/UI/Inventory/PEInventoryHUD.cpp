#include "UI/Inventory/PEInventoryHUD.h"
#include "UI/Inventory/PEInventoryBagSlot_Widget.h"
#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "UI/Inventory/PEInventoryType.h"


void UPEInventoryHUD::NativeOnInitialized()
{
	if (BagSlotWidget)
	{
		BagSlotWidget->CustomOnInitialized(this);
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->CustomOnInitialized(this);
	}
}

void UPEInventoryHUD::SetupComponentReference(UActorComponent* InIntenvoryBagComponent, UActorComponent* InEquipmentQuickSlotComponent)
{
	// Interface, To be confirmed.
	IntenvoryBagComponent = InIntenvoryBagComponent;
	EquipmentQuickSlotComponent = InEquipmentQuickSlotComponent;
}

void UPEInventoryHUD::UpdateInventoryUI(UPARAM(ref) FInventoryInfo& InInventoryInfo)
{
	if (BagSlotWidget)
	{
		BagSlotWidget->UpdateInventoryUI(InInventoryInfo);
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->UpdateInventoryUI(InInventoryInfo);
	}
}

void UPEInventoryHUD::DropItemFromBagToLand(int32 Index)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] DropItemFromBagToLand Index: %d"), Index);

	// Notify to Character Component
	// To Be Added.
}

void UPEInventoryHUD::UnequipWeaponAndDropToLand(FGameplayTag WeaponTag)
{
	FString TagString = WeaponTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] UnequipWeaponAndDropToLand Tag: %s"), *TagString);

	// Notify to Character Component
	// To Be Added.
}

void UPEInventoryHUD::RemoveItemFromQucikSlotAndDropToLand(FGameplayTag ItemTag)
{
	FString TagString = ItemTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] RemoveItemFromQucikSlotAndDropToLand Tag: %s"), *TagString);

	// Notify to Character Component
	// To Be Added.
}

void UPEInventoryHUD::AddItemFromBagToQuickSlot(FGameplayTag ItemTag, int32 FromSlotIndex)
{
	FString TagString = ItemTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] AddItemFromBagToQuickSlot Tag: %s, Index: %d"), *TagString, FromSlotIndex);

	// Notify to Character Component
	// To Be Added.
}

void UPEInventoryHUD::RemoveItemFromQuickSlotToBag(FGameplayTag ItemTag, int32 ToSlotIndex)
{
	FString TagString = ItemTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] RemoveItemFromQuickSlotToBag Tag: %s, Index: %d"), *TagString, ToSlotIndex);

	// Notify to Character Component
	// To Be Added.
}
