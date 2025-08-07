// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PEEquipmentType.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UPEEquipmentType : public UObject
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EPEEquipmentType : uint8
{
	None			UMETA(DisplayName = "None"),
	Primary			UMETA(DisplayName = "Primary Weapon"),
	Secondary		UMETA(DisplayName = "Secondary Weapon"),
	Melee			UMETA(DisplayName = "Melee Weapon"),
	Throwable		UMETA(DisplayName = "Throwable Weapon")
};
