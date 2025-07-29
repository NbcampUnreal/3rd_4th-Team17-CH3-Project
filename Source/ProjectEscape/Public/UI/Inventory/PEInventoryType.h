#pragma once

#include "CoreMinimal.h"
#include "PEInventoryType.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemCategory : uint8
{
	NONE,
	AMMO,
	HEAL_ITEM,
	GRENADE_ITEM,
	KEY_ITEM,
};

UENUM(BlueprintType)
enum class EInventoryWeaponCategory : uint8
{
	NONE,
	MAIN_WEAPON,
	SUB_WEAPON,
	MELEE_WEAPON,
};

USTRUCT(BlueprintType)
struct FInventoryItemInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemImageTexture;

	UPROPERTY(BlueprintReadWrite)
	FText ItemDescription;
};

USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FInventoryItemInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EInventoryItemCategory Category = EInventoryItemCategory::NONE;

	UPROPERTY(BlueprintReadWrite)
	int32 StackCount;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxStackCount;

	UPROPERTY(BlueprintReadWrite)
	bool IsStackable;
};

USTRUCT(BlueprintType)
struct FInventoryRangeWeaponInfo : public FInventoryItemInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EInventoryWeaponCategory Category = EInventoryWeaponCategory::NONE;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite)
	int32 TotalAmmo;
};

USTRUCT(BlueprintType)
struct FInventoryMeleeWeaponInfo : public FInventoryItemInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EInventoryWeaponCategory Category = EInventoryWeaponCategory::NONE;
};

USTRUCT(BlueprintType)
struct FInventoryInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TMap<int32, FInventoryItemInfo> Bags;

	UPROPERTY(BlueprintReadWrite)
	FInventoryRangeWeaponInfo MainWeapon;

	UPROPERTY(BlueprintReadWrite)
	FInventoryRangeWeaponInfo SubWeapon;

	UPROPERTY(BlueprintReadWrite)
	FInventoryMeleeWeaponInfo MeleeWeapon;

	UPROPERTY(BlueprintReadWrite)
	FInventoryItemInfo HealItem;

	UPROPERTY(BlueprintReadWrite)
	FInventoryItemInfo Grenade;
};
