// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "PEQuickSlotHandler.generated.h"

enum class EPEEquipmentType : uint8;
// This class does not need to be modified.
UINTERFACE()
class UPEQuickSlotHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTESCAPE_API IPEQuickSlotHandler
{
	GENERATED_BODY()

public:
	virtual void HandEquipment(EPEEquipmentType EquipmentType) = 0;
	virtual void DropHandEquipmentToWorld(const FGameplayTag& EquipmentTag) = 0;
};
