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
	virtual void OnPickedUp() = 0;
	virtual void OnDropToWorld(const FVector& Location, const FRotator& Rotation) = 0;
	virtual void OnDropToWorld(int32 Count, const FVector& Location, const FRotator& Rotation) = 0;
	virtual void SplitAndDropItem(int32 Count, const FVector& Location, const FRotator& Rotation) = 0;
	virtual int32 GetItemCount() const = 0;
	virtual int32 GetItemStackCount() const = 0;
	virtual void AddItemCount(int32 Count) = 0;
	virtual void ReduceItemCount(int32 Count) = 0;
	virtual void DestoryItem() = 0;
};
