// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEQuickSlotItemComponent.generated.h"

enum class EPEEquipmentType : uint8;
/*
 * 미정 클래스
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

public:
	// 아이템이 주워졌을 때 호출
	UFUNCTION()
	void OnItemPickedUp(AActor* NewItemOwner);

	UFUNCTION()
	void OnItemDropped(AActor* NewItemOwner);

	UFUNCTION()
	EPEEquipmentType GetEquipmentType();
};
