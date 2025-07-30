#include "UI/Inventory/PEInventoryHUD.h"
#include "UI/Inventory/PEInventoryBagSlot.h"
#include "Components/UniformGridPanel.h"


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

	InitEmptyBagSlot();
}

void UPEInventoryHUD::InitEmptyBagSlot()
{
	if (BagSlotUniformGrid && BagSlotClass)
	{
		for (int Row = 0; Row < BagRowCount; Row++)
		{
			for (int Col = 0; Col < BagColCount; Col++)
			{
				UPEInventoryBagSlot* NewSlot = CreateWidget<UPEInventoryBagSlot>(this, BagSlotClass);
				NewSlot->InitEmpty(Row * BagColCount + Col);
				BagSlotUniformGrid->AddChildToUniformGrid(NewSlot, Row, Col);
			}
		}
	}
}
