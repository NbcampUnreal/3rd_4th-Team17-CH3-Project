#include "UI/Inventory/PEInventoryBagSlot.h"
#include "UI/Inventory/PEInventoryType.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryBagSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPEInventoryBagSlot::InitEmpty(int32 InSlotIndex)
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
	}
}

void UPEInventoryBagSlot::InitSlot(int32 InSlotIndex, FInventoryItemInfo& ItemInfo)
{
	SlotIndex = InSlotIndex;
	IsVaildSlot = true;

	check(ItemInfo.Category != EInventoryItemCategory::NONE);
	
	if (StackCountText)
	{
		if (ItemInfo.IsStackable)
		{
			FString FormatString = FString::Printf(TEXT("%d"), ItemInfo.StackCount);
			StackCountText->SetText(FText::FromString(FormatString));
		}
		else
		{
			StackCountText->SetText(FText::FromString(FString("")));
		}
	}

	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(ItemInfo.ItemImageTexture);
		ItemImage->SetVisibility(ESlateVisibility::Visible);
	}
}
