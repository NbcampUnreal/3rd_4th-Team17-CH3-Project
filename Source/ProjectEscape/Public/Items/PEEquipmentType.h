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
	None		= 0	UMETA(DisplayName = "None"),
	Primary		= 1	UMETA(DisplayName = "Primary Weapon"),
	Secondary	= 2	UMETA(DisplayName = "Secondary Weapon"),
	Melee		= 3	UMETA(DisplayName = "Melee Weapon"),
	Healing		= 4	UMETA(DisplayName = "Healing Item"),
	Throwable	= 5	UMETA(DisplayName = "Throwable Weapon"),

	MAX				UMETA(Hidden)
};
