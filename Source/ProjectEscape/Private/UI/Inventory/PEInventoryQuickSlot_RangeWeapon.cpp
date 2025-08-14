#include "UI/Inventory/PEInventoryQuickSlot_RangeWeapon.h"
#include "UI/Inventory/PEInventoryType.h"
#include "Core/PEGameplayTags.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UPEInventoryQuickSlotWidget_RangeWeapon::SetSlot(FInventoryRangeWeaponInfo& WeaponInfo)
{
	const FPEGameplayTags& ItemTags = FPEGameplayTags::Get();
	if (WeaponInfo.ItemTag.MatchesTag(SlotCategoryTag))
	{
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
		SetSlotTag(WeaponInfo.ItemTag);
		SetAmmoCount(WeaponInfo.CurrentAmmo, WeaponInfo.TotalAmmo);
		SetShortCutBoxVisiblity(true);
		IsVaildSlot = true;
	}
}

void UPEInventoryQuickSlotWidget_RangeWeapon::ResetSlot()
{
	SetImageFromTexture(nullptr);
	SetAmmoCount(-1, -1);
	SetShortCutBoxVisiblity(false);
	IsVaildSlot = false;
}

void UPEInventoryQuickSlotWidget_RangeWeapon::SetAmmoCount(int32 Current, int32 Total)
{
	if (AmmoCountText)
	{
		FString FormatString = FString::Printf(TEXT("%d/%d"), Current, Total);
		AmmoCountText->SetText(FText::FromString(FormatString));
	}
}
