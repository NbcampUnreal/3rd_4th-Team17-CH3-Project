#include "Core/PEGameplayTags.h"
#include "GameplayTagsManager.h"

FPEGameplayTags FPEGameplayTags::GameplayTags;

void FPEGameplayTags::InitializeNativeTags()
{
	GameplayTags.AddAllTags();
}

void FPEGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	FName Tag = FName(TagName);
	FString Comment = FString(TEXT("(Native) ")) + FString(TagComment);
	OutTag = TagsManager.AddNativeGameplayTag(Tag, Comment);
}

void FPEGameplayTags::AddAllTags()
{
	AddTag(Item_Weapon, "Item.Weapon", "");
	AddTag(Item_Weapon_RangeWeapon, "Item.Weapon.RangeWeapon", "");
	AddTag(Item_Weapon_MeleeWeapon, "Item.Weapon.MeleeWeapon", "");
	AddTag(Item_Weapon_RangeWeapon_MainWeapon, "Item.Weapon.RangeWeapon.MainWeapon", "");
	AddTag(Item_Weapon_RangeWeapon_SubWeapon, "Item.Weapon.RangeWeapon.SubWeapon", "");
	AddTag(Item_Things, "Item.Things", "");
	AddTag(Item_Things_Ammo, "Item.Things.Ammo", "");
	AddTag(Item_Things_Ammo_Rifle, "Item.Things.Ammo.Rifle", "");
	AddTag(Item_Things_Ammo_Shotgun, "Item.Things.Ammo.Shotgun", "");
	AddTag(Item_Things_Heal, "Item.Things.Heal", "");
	AddTag(Item_Things_Grenade, "Item.Things.Grenade", "");
	AddTag(Item_Things_Key, "Item.Things.Key", "");
}
