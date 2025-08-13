// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "FPEItemData.generated.h"

class UPaperSprite;
/**
 * 
 */
USTRUCT()
struct FPEItemData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName Name = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 StackCapacity = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TObjectPtr<UTexture2D> IconTexture;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TObjectPtr<UPaperSprite> IconSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FGameplayTag ItemTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText Description = FText::GetEmpty();
};
