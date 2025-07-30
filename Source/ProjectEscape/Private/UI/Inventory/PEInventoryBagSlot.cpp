#include "UI/Inventory/PEInventoryBagSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryBagSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPEInventoryBagSlot::InitEmpty(uint32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
	IsVaildSlot = false;

	if (StackCountText)
	{
		StackCountText->SetText(FText::FromString(FString("")));
	}

	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(nullptr);
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		//ItemImage->Brush.SetResourceObject(nullptr);
	}
}

void UPEInventoryBagSlot::InitSlot(uint32 InSlotIndex, IInventoryBagItem& BagItem)
{
	SlotIndex = InSlotIndex;
	IsVaildSlot = true;
}
