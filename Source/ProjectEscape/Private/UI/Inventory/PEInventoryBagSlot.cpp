#include "UI/Inventory/PEInventoryBagSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryBagSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPEInventoryBagSlot::InitEmpty(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
	ResetSlot();
}

void UPEInventoryBagSlot::InitSlot(int32 InSlotIndex, FInventoryItemInfo& ItemInfo)
{
	check(ItemInfo.Category != EInventoryItemCategory::NONE);
	SlotIndex = InSlotIndex;
	SetSlot(ItemInfo.ItemImageTexture, ItemInfo.StackCount, ItemInfo.IsStackable);
}

void UPEInventoryBagSlot::ResetSlot()
{
	SetStackCount(-1, false);
	SetTexture(nullptr);
	IsVaildSlot = false;
}

void UPEInventoryBagSlot::SetSlot(UTexture2D* Texture, int Count, bool Stackable)
{
	SetStackCount(Count, Stackable);
	SetTexture(Texture);
	IsVaildSlot = true;
}

void UPEInventoryBagSlot::SwapSlot(UPEInventoryBagSlot* Other)
{
	if (this == Other)
	{
		return;
	}

	UTexture2D* Temp_Texture = this->ItemTexture;
	int32 Temp_Count = this->StackCount;
	bool Temp_Stackable = this->IsStackable;
	
	this->ResetSlot();
	if (Other->IsValid())
	{
		this->SetSlot(Other->ItemTexture, Other->StackCount, Other->IsStackable);
	}
	Other->SetSlot(Temp_Texture, Temp_Count, Temp_Stackable);
}

void UPEInventoryBagSlot::SetTexture(UTexture2D* Texture)
{
	ItemTexture = Texture;
	if (ItemImage)
	{
		if (ItemTexture)
		{
			ItemImage->SetBrushFromTexture(ItemTexture);
			ItemImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemImage->SetBrushFromTexture(nullptr);
			ItemImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPEInventoryBagSlot::SetStackCount(int Count, bool Stackable)
{
	StackCount = Count;
	IsStackable = Stackable;
	if (StackCountText)
	{
		if (IsStackable)
		{
			FString FormatString = FString::Printf(TEXT("%d"), StackCount);
			StackCountText->SetText(FText::FromString(FormatString));
		}
		else
		{
			StackCountText->SetText(FText::FromString(FString("")));
		}
	}
}
