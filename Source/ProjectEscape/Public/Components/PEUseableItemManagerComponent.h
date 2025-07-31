// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEUseableItemManagerComponent.generated.h"


class UPEUseableComponent;
class IPEUseable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEUseableItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPEUseableItemManagerComponent();

protected:
	virtual void BeginPlay() override;

	/* 아이템 관련 섹션 */
	UPROPERTY(VisibleAnywhere, Category = "Useable")
	TObjectPtr<UPEUseableComponent> CurrentItemComponent; // 현재 손에 들고 있는 아이템
	
	UPROPERTY(VisibleAnywhere, Category = "Useable")
	TObjectPtr<AActor> OwnerActor; // 현재 손에 들고 있는 아이템

public:
	//void SetHandItem(IPEUseable* NewItem);
	void SetHandItem(UPEUseableComponent* NewItemComponent);
	UPEUseableComponent* GetCurrentItem() const;
	void UseCurrentItem(AActor* Holder);
};