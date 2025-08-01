#include "UI/Inventory/PEInventoryQuickSlot_MeleeWeapon.h"
#include "UI/Inventory/PEInventoryType.h"
#include "Core/PEGameplayTags.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryQuickSlotWidget_MeleeWeapon::SetSlot(FInventoryMeleeWeaponInfo& WeaponInfo)
{
	const FPEGameplayTags& ItemTags = FPEGameplayTags::Get();
	if (WeaponInfo.ItemTag.MatchesTag(ItemTags.Item_Weapon_MeleeWeapon))
	{
		SetSlotTag(WeaponInfo.ItemTag);
		if (WeaponInfo.ItemTexture)
		{
			SetImageFromTexture(WeaponInfo.ItemTexture);
		}
		else if (WeaponInfo.ItemSprite)
		{
			SetImageFromSprite(WeaponInfo.ItemSprite);
		}
		else
		{
			SetImageFromTexture(nullptr);
		}
		SetShortCutBoxVisiblity(true);
		IsVaildSlot = true;
	}
}

void UPEInventoryQuickSlotWidget_MeleeWeapon::ResetSlot()
{
	SetImageFromTexture(nullptr);
	SetShortCutBoxVisiblity(false);
	IsVaildSlot = false;
}
