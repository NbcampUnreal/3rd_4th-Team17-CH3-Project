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
