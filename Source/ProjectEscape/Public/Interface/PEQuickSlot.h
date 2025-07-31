// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEQuickSlot.generated.h"

enum class EPEEquipmentType : uint8;
// This class does not need to be modified.
UINTERFACE()
class UPEQuickSlot : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTESCAPE_API IPEQuickSlot
{
	GENERATED_BODY()

public:
	virtual void HandEquipment(EPEEquipmentType EquipmentType) = 0;
};
