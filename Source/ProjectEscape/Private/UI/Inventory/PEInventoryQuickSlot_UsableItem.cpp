#include "UI/Inventory/PEInventoryQuickSlot_UsableItem.h"
#include "UI/Inventory/PEInventoryType.h"
#include "Core/PEGameplayTags.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryQuickSlotWidget_UsableItem::SetSlot(FInventoryUsableItemInfo& ItemInfo)
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
		SetShortCutBoxVisiblity(true);
		IsVaildSlot = true;
	}
}

void UPEInventoryQuickSlotWidget_UsableItem::ResetSlot()
{
	SetImageFromTexture(nullptr);
	SetStackCount(-1, -1, false);
	SetShortCutBoxVisiblity(true);
	IsVaildSlot = false;
}

void UPEInventoryQuickSlotWidget_UsableItem::SetStackCount(int32 Stack, int32 MaxStack, bool Stackable)
{
	if (StackCountText)
	{
		if (Stackable)
		{
			FString FormatString = FString::Printf(TEXT("%d"), Stack);
			StackCountText->SetText(FText::FromString(FormatString));
		}
		else
		{
			StackCountText->SetText(FText::FromString(FString("")));
		}
	}
}
