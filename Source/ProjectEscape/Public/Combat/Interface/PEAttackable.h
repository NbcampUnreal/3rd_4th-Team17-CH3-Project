// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEAttackable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPEAttackable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTESCAPE_API IPEAttackable
{
	GENERATED_BODY()

public:
	virtual USceneComponent* GetAttackStartPoint() const = 0;
};
