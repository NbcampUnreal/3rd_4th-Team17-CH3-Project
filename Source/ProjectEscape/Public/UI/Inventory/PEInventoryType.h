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

USTRUCT(BlueprintType)
struct FInventoryItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 StackCount;

	UPROPERTY(BlueprintReadWrite)
	bool IsStackable;

	UPROPERTY(BlueprintReadWrite)
	EInventoryItemCategory Category;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemImageTexture;

	UPROPERTY(BlueprintReadWrite)
	FText ItemDescription;
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
struct FInventoryRangeWeaponInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EInventoryWeaponCategory Category;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemImageTexture;

	UPROPERTY(BlueprintReadWrite)
	FText ItemDescription;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite)
	int32 TotalAmmo;
};

USTRUCT(BlueprintType)
struct FInventoryMeleeWeaponInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EInventoryWeaponCategory Category;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemImageTexture;

	UPROPERTY(BlueprintReadWrite)
	FText ItemDescription;
};
