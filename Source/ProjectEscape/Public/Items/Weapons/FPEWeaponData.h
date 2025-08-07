// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPEWeaponData.generated.h"

enum class EPEEquipmentType : uint8;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FPEWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPEEquipmentType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BulletType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAutomatic;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsHitscan;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMagazineReload;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BulletsPerShot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;

	UPROPERTY()
	float ReloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};