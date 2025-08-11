// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PEEquipmentType.h"
#include "UObject/Interface.h"
#include "PEQuickSlotItem.generated.h"

class UPEQuickSlotItemComponent;
// This class does not need to be modified.
UINTERFACE()
class UPEQuickSlotItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTESCAPE_API IPEQuickSlotItem
{
	GENERATED_BODY()

public:
	virtual AActor* GetItemOwner() const = 0;
	virtual void OnDropped() = 0;
	virtual UPEQuickSlotItemComponent* GetQuickSlotItemComponent() const = 0;
	virtual EPEEquipmentType GetEquipmentType() const = 0;
};
