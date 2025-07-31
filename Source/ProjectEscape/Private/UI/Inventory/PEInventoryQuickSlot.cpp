#include "UI/Inventory/PEInventoryQuickSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

/* Slot Base */

void UPEInventoryQuickSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ShortCutText)
	{
		ShortCutText->SetText(FText::FromString(ShortCutString));
	}
}

void UPEInventoryQuickSlot::InitEmpty(EInventoryItemCategory InCategory)
{
	Category = InCategory;
	ResetSlot();
}

void UPEInventoryQuickSlot::SetTexture(UTexture2D* Texture)
{
	WeaponTexture = Texture;
	if (ItemImage)
	{
		if (WeaponTexture)
		{
			ItemImage->SetBrushFromTexture(WeaponTexture);
			ItemImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemImage->SetBrushFromTexture(nullptr);
			ItemImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UPEInventoryQuickSlot::SetShortCutBoxVisiblity(bool Visibile)
{
	if (ShortCutBox)
	{
		if (Visibile)
		{
			ShortCutBox->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ShortCutBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


/* Ranged Weapon */

void UPEInventoryRangeWeaponSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPEInventoryRangeWeaponSlot::InitSlot(FInventoryRangeWeaponInfo& WeaponInfo)
{
	if (WeaponInfo.Category != EInventoryItemCategory::NONE)
	{
		check(WeaponInfo.Category == Category);
		SetSlot(WeaponInfo.ItemImageTexture, WeaponInfo.CurrentAmmo, WeaponInfo.TotalAmmo);
	}	
}

void UPEInventoryRangeWeaponSlot::ResetSlot()
{
	SetTexture(nullptr);
	SetShortCutBoxVisiblity(false);
	SetAmmoCount(-1, -1);
	IsVaildSlot = false;
}

void UPEInventoryRangeWeaponSlot::SetSlot(UTexture2D* Texture, int32 CurrentAmmo, int32 TotalAmmo)
{
	SetTexture(Texture);
	SetAmmoCount(CurrentAmmo, TotalAmmo);
	SetShortCutBoxVisiblity(true);
	IsVaildSlot = true;
}

void UPEInventoryRangeWeaponSlot::SetAmmoCount(int32 Current, int32 Total)
{
	CurrentAmmoCount = Current;
	TotalAmmoCount = Total;
	if (AmmoCountText)
	{
		if (Current > 0 && Total > 0)
		{
			FString FormatString = FString::Printf(TEXT("%d/%d"), Current, Total);
			AmmoCountText->SetText(FText::FromString(FormatString));
		}
		else
		{
			AmmoCountText->SetText(FText::FromString(FString("")));
		}
	}
}

/* Melee Weapon */

void UPEInventoryMeleeWeaponSlot::InitSlot(FInventoryMeleeWeaponInfo& WeaponInfo)
{
	if (WeaponInfo.Category != EInventoryItemCategory::NONE)
	{
		check(WeaponInfo.Category == Category);
		SetSlot(WeaponInfo.ItemImageTexture);
	}
}

void UPEInventoryMeleeWeaponSlot::ResetSlot()
{
	SetTexture(nullptr);
	SetShortCutBoxVisiblity(false);
	IsVaildSlot = false;
}

void UPEInventoryMeleeWeaponSlot::SetSlot(UTexture2D* Texture)
{
	SetTexture(Texture);
	SetShortCutBoxVisiblity(true);
	IsVaildSlot = true;
}


/* Heal and Grenade */

void UPEInventoryQuickItemSlot::InitSlot(FInventoryItemInfo& ItemInfo)
{
	if (ItemInfo.Category != EInventoryItemCategory::NONE)
	{
		check(ItemInfo.Category == Category);
		SetSlot(ItemInfo.ItemImageTexture, ItemInfo.StackCount, ItemInfo.MaxStackCount, ItemInfo.IsStackable);
	}
}

void UPEInventoryQuickItemSlot::ResetSlot()
{
	SetTexture(nullptr);
	SetShortCutBoxVisiblity(false);
	SetStackCount(-1, -1, false);
	IsVaildSlot = false;
}

void UPEInventoryQuickItemSlot::SetSlot(UTexture2D* Texture, int32 Stack, int32 MaxStack, bool Stackable)
{
	SetTexture(Texture);
	SetStackCount(Stack, MaxStack, Stackable);
	SetShortCutBoxVisiblity(true);
	IsVaildSlot = true;
}

void UPEInventoryQuickItemSlot::SetStackCount(int32 Stack, int32 MaxStack, bool Stackable)
{
	StackCount = Stack;
	MaxStackCount = MaxStack;
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
