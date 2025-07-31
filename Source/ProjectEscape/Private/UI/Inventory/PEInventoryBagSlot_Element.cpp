#include "UI/Inventory/PEInventoryBagSlot_Element.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UPEInventoryBagSlot_Element::SetSlot(FInventoryBagSlotInfo& ItemInfo)
{
	const FPEGameplayTags& ItemTags = FPEGameplayTags::Get();
	//if (ItemInfo.ItemTag == ItemTags.Item_Things)
	{
		if (ItemInfo.ItemTexture)
		{
			SetImageFromTexture(ItemInfo.ItemTexture);
		}
		else if (ItemInfo.ItemSprite)
		{
			SetImageFromSprite(ItemInfo.ItemSprite);
		}
		else
		{
			SetImageFromTexture(nullptr);
		}
		SetStackCount(ItemInfo.StackCount, ItemInfo.MaxStackCount, ItemInfo.IsStackable);
		IsVaildSlot = true;
	}
}

void UPEInventoryBagSlot_Element::ResetSlot()
{
	SetImageFromTexture(nullptr);
	SetStackCount(-1, -1, false);
	IsVaildSlot = false;
}

void UPEInventoryBagSlot_Element::SetImageFromTexture(UTexture2D* Texture)
{
}

void UPEInventoryBagSlot_Element::SetImageFromSprite(UPaperSprite* Sprite)
{
}

void UPEInventoryBagSlot_Element::SetStackCount(int Count, int MaxCount, bool Stackable)
{
	if (StackCountText)
	{
		if (Stackable)
		{
			FString FormatString = FString::Printf(TEXT("%d"), Count);
			StackCountText->SetText(FText::FromString(FormatString));
		}
		else
		{
			StackCountText->SetText(FText::FromString(FString("")));
		}
	}
}
