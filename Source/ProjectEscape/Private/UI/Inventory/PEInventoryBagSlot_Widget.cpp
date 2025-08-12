#include "UI/Inventory/PEInventoryBagSlot_Widget.h"
#include "UI/Inventory/PEInventoryBagSlot_Element.h"
#include "UI/Inventory/PEInventoryType.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UPEInventoryBagSlot_Widget::CreateEmptyBagSlots()
{
	if (UniformGridPanel && BagSlotClass)
	{
		for (int Row = 0; Row < BagRowCount; Row++)
		{
			for (int Col = 0; Col < BagColCount; Col++)
			{
				UPEInventoryBagSlot_Element* NewElement = CreateWidget<UPEInventoryBagSlot_Element>(this, BagSlotClass);
				int32 Index = Row * BagColCount + Col;
				NewElement->SetParentWidget(this->ParentWidget);
				NewElement->SetSlotIndex(Index);

				UniformGridPanel->AddChildToUniformGrid(NewElement, Row, Col);
				BagSlots.Add(Index, NewElement);
			}
		}
	}
}

void UPEInventoryBagSlot_Widget::CustomOnInitialized(UPEInventoryHUD* Parent)
{
	SetParentWidget(Parent);
	CreateEmptyBagSlots();
}

void UPEInventoryBagSlot_Widget::UpdateInventoryUI(FInventoryInfo& InInventoryInfo)
{
	ResetBagSlots();

	for (auto& BagInfo : InInventoryInfo.Bags)
	{
		int32 Index = BagInfo.Key;
		FInventoryBagSlotInfo& ItemInfo = BagInfo.Value;

		if (BagSlots.Contains(Index))
		{
			if (UPEInventoryBagSlot_Element* BagSlot = BagSlots[Index])
			{
				BagSlot->SetSlot(ItemInfo);
			}
		}
	}
}

void UPEInventoryBagSlot_Widget::ResetBagSlots()
{
	for (int Row = 0; Row < BagRowCount; Row++)
	{
		for (int Col = 0; Col < BagColCount; Col++)
		{
			int32 Index = Row * BagColCount + Col;
			ResetSlot(Index);
		}
	}
}

void UPEInventoryBagSlot_Widget::ResetSlot(int32 Index)
{
	if (BagSlots.Contains(Index))
	{
		if (UPEInventoryBagSlot_Element* BagSlot = BagSlots[Index])
		{
			BagSlot->ResetSlot();
		}
	}
}
