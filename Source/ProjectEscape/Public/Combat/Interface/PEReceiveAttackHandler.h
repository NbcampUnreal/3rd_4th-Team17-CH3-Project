// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PEReceiveAttackHandler.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPEReceiveAttackHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 *	액터가 공격받을 때 처리를 위한 인터페이스입니다.
 *	현재는 TakeDamage같은 함수가 존재하기 때문에 사용하지 않습니다.
 */
class PROJECTESCAPE_API IPEReceiveAttackHandler
{
	GENERATED_BODY()

public:
};
