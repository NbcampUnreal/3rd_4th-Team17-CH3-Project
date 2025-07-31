#include "UI/Inventory/PEInventoryBagSlot_Widget.h"
#include "UI/Inventory/PEInventoryBagSlot_Element.h"
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
				//NewElement->InitEmpty(Index);

				UniformGridPanel->AddChildToUniformGrid(NewElement, Row, Col);
				BagSlots.Add(Index, NewElement);
			}
		}
	}
}

void UPEInventoryBagSlot_Widget::ResetBagSlots()
{
}

void UPEInventoryBagSlot_Widget::InitBagSlots()
{
}

void UPEInventoryBagSlot_Widget::ResetSlot(int32 Index)
{
}

void UPEInventoryBagSlot_Widget::SwapSlot(int32 Index, int32 OtherIndex)
{
}


