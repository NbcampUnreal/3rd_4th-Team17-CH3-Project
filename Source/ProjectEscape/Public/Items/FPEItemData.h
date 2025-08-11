// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPEItemData.generated.h"

/**
 * 
 */
USTRUCT()
struct FPEItemData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 StackCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FString Description;
};
