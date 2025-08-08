// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEUseable.generated.h"

class UPEUseableComponent;
// This class does not need to be modified.
UINTERFACE()
class UPEUseable : public UInterface
{
	GENERATED_BODY()
};

/**
 *  손에 들고 사용해야 하는 액터(아이템)들을 위한 인터페이스입니다
 */
class PROJECTESCAPE_API IPEUseable
{
	GENERATED_BODY()

public:
	virtual void DoPrimaryAction(AActor* Holder) = 0;
	virtual void CompletePrimaryAction(AActor* Holder) = 0;
	virtual void DoSecondaryAction(AActor* Holder) = 0;
	virtual void DoTertiaryAction(AActor* Holder) = 0;
	virtual void OnHand(AActor* NewOwner) = 0;
	virtual UPEUseableComponent* GetUseableComponent() const = 0; // 사용 가능한 컴포넌트를 반환합니다
};
