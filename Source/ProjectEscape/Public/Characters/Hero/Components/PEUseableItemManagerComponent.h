// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEUseableItemManagerComponent.generated.h"


class UPEUseableComponent;
class IPEUseable;

/*
 *	손에 들고 있는 아이템을 관리하는 컴포넌트 클래스입니다.
 *	구현되어야 하는 기능:
 *	1. 아이템을 손에 드는 기능 (Actor Reference)
 *	2. 아이템을 해제하는 기능 (Actor Reference 해제)
 *	3. 현재 손에 들고 있는 아이템을 반환하는 기능
 *	4. 현재 손에 들고 있는 아이템을 사용하는 기능
 *	
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEUseableItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
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
	void SetHandItem(UPEUseableComponent* NewItemComponent);
	UPEUseableComponent* GetCurrentItem() const;
	void ReleaseHandItem();
	void UseCurrentItem(AActor* Holder);
};
