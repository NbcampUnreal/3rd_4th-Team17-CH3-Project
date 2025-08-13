// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FPEEquipmentInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct PROJECTESCAPE_API FPEEquipmentInfo 
{
	GENERATED_BODY()

	UPROPERTY()
	FName EquipmentName;

	UPROPERTY()
	FString AmmoCount;

	UPROPERTY()
	FString EquipmentDescription;

	UPROPERTY()
	TSoftObjectPtr<UTexture2D> EquipmentIcon;
};
