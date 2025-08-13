#pragma once

#include "CoreMinimal.h"
#include "Core/PEGameplayTags.h"
#include "PEInventoryType.generated.h"

class UTexture2D;
class UPaperSprite;
struct FSlateBrush;

USTRUCT(BlueprintType)
struct FInventoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTexture2D> ItemTexture;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UPaperSprite> ItemSprite;

	UPROPERTY(BlueprintReadWrite)
	FText ItemDescription;

	UPROPERTY(BlueprintReadWrite, meta = ( Categories = "Item") )
	FGameplayTag ItemTag;
};

USTRUCT(BlueprintType)
struct FInventoryBagSlotInfo : public FInventoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 StackCount;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxStackCount;

	UPROPERTY(BlueprintReadWrite)
	bool IsStackable;
};

USTRUCT(BlueprintType)
struct FInventoryRangeWeaponInfo : public FInventoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(BlueprintReadWrite)
	int32 TotalAmmo;
};

USTRUCT(BlueprintType)
struct FInventoryMeleeWeaponInfo : public FInventoryInfoBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FInventoryUsableItemInfo : public FInventoryInfoBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 StackCount;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxStackCount;

	UPROPERTY(BlueprintReadWrite)
	bool IsStackable;
};

USTRUCT(BlueprintType)
struct FInventoryInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FInventoryBagSlotInfo> Bags;

	UPROPERTY(BlueprintReadWrite)
	FInventoryRangeWeaponInfo MainWeapon;

	UPROPERTY(BlueprintReadWrite)
	FInventoryRangeWeaponInfo SubWeapon;

	UPROPERTY(BlueprintReadWrite)
	FInventoryMeleeWeaponInfo MeleeWeapon;

	UPROPERTY(BlueprintReadWrite)
	FInventoryUsableItemInfo QuickHeal;

	UPROPERTY(BlueprintReadWrite)
	FInventoryUsableItemInfo QuickGrenade;
};
