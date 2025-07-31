#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "UI/Inventory/PEInventoryQuickSlot_RangeWeapon.h"
#include "UI/Inventory/PEInventoryQuickSlot_MeleeWeapon.h"
#include "UI/Inventory/PEInventoryQuickSlot_UsableItem.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryQuickSlot_Widget::NativeOnInitialized()
{
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

void UPEInventoryQuickSlot_SlotBase::SetImageFromTexture(UTexture2D* Texture)
{
}

void UPEInventoryQuickSlot_SlotBase::SetImageFromSprite(UPaperSprite* Sprite)
{
}

void UPEInventoryQuickSlot_SlotBase::SetShortCutBoxVisiblity(bool Visibile)
{
}
