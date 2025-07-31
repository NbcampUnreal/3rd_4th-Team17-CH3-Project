#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "UI/Inventory/PEInventoryQuickSlot_RangeWeapon.h"
#include "UI/Inventory/PEInventoryQuickSlot_MeleeWeapon.h"
#include "UI/Inventory/PEInventoryQuickSlot_UsableItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"

void UPEInventoryQuickSlot_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (MainWeaponSlot)
	{
		MainWeaponSlot->SetParentWidget(ParentWidget);
	}
	if (SubWeaponSlot)
	{
		SubWeaponSlot->SetParentWidget(ParentWidget);
	}
	if (MeleeWeaponSlot)
	{
		MeleeWeaponSlot->SetParentWidget(ParentWidget);
	}
	if (HealItemSlot)
	{
		HealItemSlot->SetParentWidget(ParentWidget);
	}
	if (GrenadeSlot)
	{
		GrenadeSlot->SetParentWidget(ParentWidget);
	}
}

void UPEInventoryQuickSlot_Widget::UpdateInventoryUI(FInventoryInfo& InInventoryInfo)
{
	if (MainWeaponSlot)
	{
		MainWeaponSlot->ResetSlot();
		MainWeaponSlot->SetSlot(InInventoryInfo.MainWeapon);
	}
	if (SubWeaponSlot)
	{
		SubWeaponSlot->ResetSlot();
		SubWeaponSlot->SetSlot(InInventoryInfo.SubWeapon);
	}
	if (MeleeWeaponSlot)
	{
		MeleeWeaponSlot->ResetSlot();
		MeleeWeaponSlot->SetSlot(InInventoryInfo.MeleeWeapon);
	}
	if (HealItemSlot)
	{
		HealItemSlot->ResetSlot();
		HealItemSlot->SetSlot(InInventoryInfo.QuickHeal);
	}
	if (GrenadeSlot)
	{
		GrenadeSlot->ResetSlot();
		GrenadeSlot->SetSlot(InInventoryInfo.QuickGrenade);
	}
}

void UPEInventoryQuickSlot_SlotBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ShortCutText)
	{
		ShortCutText->SetText(FText::FromString(ShortCutString));
	}
}

void UPEInventoryQuickSlot_SlotBase::SetImageFromTexture(UTexture2D* Texture)
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

void UPEInventoryQuickSlot_SlotBase::SetImageFromSprite(UPaperSprite* Sprite)
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

void UPEInventoryQuickSlot_SlotBase::SetShortCutBoxVisiblity(bool Visibile)
{
	if (ShortCutBox)
	{
		if (Visibile)
		{
			ShortCutBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			ShortCutBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
