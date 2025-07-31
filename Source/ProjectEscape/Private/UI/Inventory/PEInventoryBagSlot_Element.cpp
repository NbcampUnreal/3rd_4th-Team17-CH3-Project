#include "UI/Inventory/PEInventoryBagSlot_Element.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"


void UPEInventoryBagSlot_Element::SetSlot(int32 Index, FInventoryBagSlotInfo& ItemInfo)
{
	SlotIndex = Index;

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
	if (ItemImage)
	{
		if (Texture)
		{
			ItemImage->SetBrushFromTexture(Texture);
			ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			ItemImage->SetBrushFromTexture(nullptr);
			ItemImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPEInventoryBagSlot_Element::SetImageFromSprite(UPaperSprite* Sprite)
{
	if (ItemImage)
	{
		// TODO Sprite를 이용한 SetBrush가 먹히지 않음..
		if (Sprite)
		{
			const FSlateAtlasData SpriteAtlasData = Sprite->GetSlateAtlasData();
			const FVector2D SpriteSize = SpriteAtlasData.GetSourceDimensions();
			FSlateBrush Brush;
			Brush.SetResourceObject(Sprite);
			Brush.ImageSize = FVector2D(SpriteSize.X, SpriteSize.Y);
			ItemImage->SetBrush(Brush);
		}
	}
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
