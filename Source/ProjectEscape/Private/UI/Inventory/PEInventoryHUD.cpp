#include "UI/Inventory/PEInventoryHUD.h"
#include "UI/Inventory/PEInventoryBagSlot.h"
#include "UI/Inventory/PEInventoryType.h"
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

	InitEmptyBagSlots();
	InitBagSlots();
}

void UPEInventoryHUD::DropItemFromBagToLand(int32 Index)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] DropItemFromBagToLand Index: %d"), Index);
}

void UPEInventoryHUD::SwapItemInBag(int32 Index, int32 OhterIndex)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] SwapItemInBag Index: %d, OhterIndex: %d"), Index, OhterIndex);
}


void UPEInventoryHUD::InitEmptyBagSlots()
{
	if (BagSlotUniformGrid && BagSlotClass)
	{
		for (int Row = 0; Row < BagRowCount; Row++)
		{
			for (int Col = 0; Col < BagColCount; Col++)
			{
				UPEInventoryBagSlot* NewSlot = CreateWidget<UPEInventoryBagSlot>(this, BagSlotClass);
				int32 Index = Row * BagColCount + Col;
				NewSlot->InitEmpty(Index);

				BagSlotUniformGrid->AddChildToUniformGrid(NewSlot, Row, Col);
				BagSlots.Add(Index, NewSlot);
			}
		}
	}
}

void UPEInventoryHUD::InitBagSlots()
{
	if (BagSlotUniformGrid)
	{
		for (auto& BagInfo : InventoryInfo.Bags)
		{
			int32 Index = BagInfo.Key;
			FInventoryItemInfo& ItemInfo = BagInfo.Value;

			if (BagSlots.Contains(Index))
			{
				if (UPEInventoryBagSlot* BagSlot = Cast<UPEInventoryBagSlot>(BagSlots[Index]))
				{
					BagSlot->InitSlot(Index, ItemInfo);
				}
			}
		}
	}
}

void UPEInventoryHUD::ResetSlot(int32 Index)
{
}
