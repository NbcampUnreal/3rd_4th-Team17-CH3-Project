#include "UI/Inventory/PEInventoryHUD.h"


UPEInventoryHUD::UPEInventoryHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BagRowCount = 2;
	BagColCount = 5;
}

void UPEInventoryHUD::NativeOnInitialized()
{
}

void UPEInventoryHUD::InitInventoryUI(FInventoryInfo InInventoryInfo)
{
	InventoryInfo = InInventoryInfo;
}
