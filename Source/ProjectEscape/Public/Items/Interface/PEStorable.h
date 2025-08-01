// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEStorable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPEStorable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTESCAPE_API IPEStorable
{
	GENERATED_BODY()

public:
	virtual bool	CanBeStored() const = 0;
	virtual bool	StoreItem(class AActor* Inventory) = 0;
	virtual bool	RemoveFromStorage(class AActor* Inventory) = 0;
	virtual int32	GetStackSize() const = 0;
	virtual int32	GetMaxStackSize() const = 0;
	virtual bool	AddToStack(int32 Amount) = 0;
	virtual FString GetItemID() const = 0;
	virtual bool	IsStackable() const = 0;
	virtual bool	DropToWorld(const FVector& Location, const FRotator& Rotation) = 0;
};
