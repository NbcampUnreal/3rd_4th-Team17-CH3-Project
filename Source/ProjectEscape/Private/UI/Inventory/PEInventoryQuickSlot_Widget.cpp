#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "UI/Inventory/PEInventoryQuickSlot_RangeWeapon.h"
#include "UI/Inventory/PEInventoryQuickSlot_MeleeWeapon.h"
#include "UI/Inventory/PEInventoryQuickSlot_UsableItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "PaperSprite.h"
#include "Core/PEGameplayTags.h"

void UPEInventoryQuickSlot_Widget::CustomOnInitialized(UPEInventoryHUD* Parent)
{
	const FPEGameplayTags& ItemTags = FPEGameplayTags::Get();
	SetParentWidget(Parent);
	if (MainWeaponSlot)
	{
		MainWeaponSlot->SetParentWidget(ParentWidget);
		MainWeaponSlot->SetSlotCategoryTag(ItemTags.Item_Weapon_RangeWeapon_MainWeapon);
	}
	if (SubWeaponSlot)
	{
		SubWeaponSlot->SetParentWidget(ParentWidget);
		SubWeaponSlot->SetSlotCategoryTag(ItemTags.Item_Weapon_RangeWeapon_SubWeapon);
	}
	if (MeleeWeaponSlot)
	{
		MeleeWeaponSlot->SetParentWidget(ParentWidget);
		MeleeWeaponSlot->SetSlotCategoryTag(ItemTags.Item_Weapon_MeleeWeapon);
	}
	if (HealItemSlot)
	{
		HealItemSlot->SetParentWidget(ParentWidget);
		HealItemSlot->SetSlotCategoryTag(ItemTags.Item_Things_Heal);
	}
	if (GrenadeSlot)
	{
		GrenadeSlot->SetParentWidget(ParentWidget);
		GrenadeSlot->SetSlotCategoryTag(ItemTags.Item_Things_Grenade);
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
