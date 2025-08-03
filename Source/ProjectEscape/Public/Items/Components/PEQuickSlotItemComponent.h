// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEQuickSlotItemComponent.generated.h"

class IPEQuickSlotItem;
enum class EPEEquipmentType : uint8;
/*
 * 퀵슬롯에 등록될 수 있는 아이템을 위한 컴포넌트 클래스입니다.
 * 구현되어야 할 기능:
 * 1. 아이템이 주워졌을 때 호출되는 함수
 * 2. 아이템이 떨어졌을 때 호출되는 함수
 * 3. 아이템의 장비 타입을 반환하는 함수
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEQuickSlotItemComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEQuickSlotItemComponent();

protected:
	virtual void BeginPlay() override;

	/* Quick Slot 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quick Slot")
	TObjectPtr<AActor> ComponentOwnerActor; // 컴포넌트 소유 액터

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quick Slot")
	TScriptInterface<IPEQuickSlotItem> ComponentOwnerActorInterface; // 컴포넌트 소유 액터 인터페이스
	
	
public:
	// 아이템이 주워졌을 때 호출
	UFUNCTION()
	void OnItemPickedUp() const;

	UFUNCTION()
	void OnItemDropped() const;

	UFUNCTION()
	EPEEquipmentType GetEquipmentType() const;

	void SetComponentInterface(UObject* NewOwner);
};
