#pragma once

#include "Containers/UnrealString.h"
#include "Containers/Map.h"
#include "GameplayTagContainer.h"

class UGameplayTagManager;

struct FPEGameplayTags
{
	static const FPEGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();

	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);
	void AddAllTags();

	/* Item Category */
	FGameplayTag Item_Weapon;
	FGameplayTag Item_Weapon_RangeWeapon;
	FGameplayTag Item_Weapon_MeleeWeapon;
	FGameplayTag Item_Weapon_RangeWeapon_MainWeapon;
	FGameplayTag Item_Weapon_RangeWeapon_SubWeapon;
	FGameplayTag Item_Things;
	FGameplayTag Item_Things_Ammo;
	FGameplayTag Item_Things_Heal;
	FGameplayTag Item_Things_Grenade;
	FGameplayTag Item_Things_Key;

private:
	static FPEGameplayTags GameplayTags;
};

