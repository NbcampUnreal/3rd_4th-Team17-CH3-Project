#include "UI/Inventory/PEInventoryHUD.h"
#include "UI/Inventory/PEInventoryBagSlot_Widget.h"
#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "UI/Inventory/PEInventoryType.h"


void UPEInventoryHUD::NativeOnInitialized()
{
	if (BagSlotWidget)
	{
		BagSlotWidget->CreateEmptyBagSlots();
		BagSlotWidget->SetParentWidget(this);
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->SetParentWidget(this);
	}
}

void UPEInventoryHUD::InitInventoryUI(FInventoryInfo InInventoryInfo)
{
	InventoryInfo = InInventoryInfo;

	/*if (BagSlots.IsEmpty())
	{
		CreateEmptyBagSlots();
	}
	else
	{
		ResetBagSlots();
	}
	InitBagSlots();

	if (MainWeaponSlot)
	{
		MainWeaponSlot->InitEmpty(EInventoryItemCategory::MAIN_WEAPON);
		MainWeaponSlot->InitSlot(InventoryInfo.MainWeapon);
	}
	if (SubWeaponSlot)
	{
		SubWeaponSlot->InitEmpty(EInventoryItemCategory::SUB_WEAPON);
		SubWeaponSlot->InitSlot(InventoryInfo.SubWeapon);
	}
	if (MeleeWeaponSlot)
	{
		MeleeWeaponSlot->InitEmpty(EInventoryItemCategory::MELEE_WEAPON);
		MeleeWeaponSlot->InitSlot(InventoryInfo.MeleeWeapon);
	}
	if (HealItemSlot)
	{
		HealItemSlot->InitEmpty(EInventoryItemCategory::HEAL_ITEM);
		HealItemSlot->InitSlot(InventoryInfo.HealItem);
	}
	if (GrenadeSlot)
	{
		GrenadeSlot->InitEmpty(EInventoryItemCategory::GRENADE_ITEM);
		GrenadeSlot->InitSlot(InventoryInfo.Grenade);
	}*/
}

void UPEInventoryHUD::DropItemFromBagToLand(int32 Index)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] DropItemFromBagToLand Index: %d"), Index);

	// Remove From UI
	//ResetSlot(Index);

	// Notify to Character Component
	// To Be Added.
}

void UPEInventoryHUD::SwapItemInBag(int32 Index, int32 OhterIndex)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] SwapItemInBag Index: %d, OhterIndex: %d"), Index, OhterIndex);

	// Swap In UI
	//SwapSlot(Index, OhterIndex);

	// Notify to Character Component
	// To Be Added.
}
