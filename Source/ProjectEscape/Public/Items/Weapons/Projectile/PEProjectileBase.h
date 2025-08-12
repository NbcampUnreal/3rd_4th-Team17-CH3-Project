// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEProjectileBase.generated.h"

UCLASS()
class PROJECTESCAPE_API APEProjectileBase : public AActor
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEProjectileBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};

