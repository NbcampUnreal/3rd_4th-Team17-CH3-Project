#include "UI/Inventory/PEInventoryWeaponSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

/* Slot Base */

void UPEInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ShortCutText)
	{
		ShortCutText->SetText(FText::FromString(ShortCutString));
	}
}

void UPEInventorySlot::InitEmpty(EInventoryWeaponCategory InCategory)
{
	Category = Category;
	ResetSlot();
}

void UPEInventorySlot::InitSlot(FInventoryRangeWeaponInfo& WeaponInfo)
{
}

void UPEInventorySlot::ResetSlot()
{
	SetTexture(nullptr);
	SetShortCutBoxVisiblity(false);
	IsVaildSlot = false;
}

void UPEInventorySlot::SetTexture(UTexture2D* Texture)
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

void UPEInventorySlot::SetShortCutBoxVisiblity(bool Visibile)
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
	check(WeaponInfo.Category != EInventoryWeaponCategory::NONE);
	check(WeaponInfo.Category == Category);
	SetSlot(WeaponInfo.ItemImageTexture, WeaponInfo.CurrentAmmo, WeaponInfo.TotalAmmo);
}

void UPEInventoryRangeWeaponSlot::ResetSlot()
{
	Super::ResetSlot();
	SetAmmoCount(-1, -1);
	IsVaildSlot = false;
}

void UPEInventoryRangeWeaponSlot::SetSlot(UTexture2D* Texture, int CurrentAmmo, int TotalAmmo)
{
	SetTexture(Texture);
	SetAmmoCount(CurrentAmmo, TotalAmmo);
	SetShortCutBoxVisiblity(true);
	IsVaildSlot = true;
}

void UPEInventoryRangeWeaponSlot::SetAmmoCount(int Current, int Total)
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
