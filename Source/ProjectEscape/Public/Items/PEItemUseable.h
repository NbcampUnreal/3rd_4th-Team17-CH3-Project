// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEItemBase.h"
#include "Interface/PEQuickSlotItem.h"
#include "Interface/PEUseable.h"
#include "Weapons/FPEEquipmentInfo.h"
#include "Weapons/FPEWeaponData.h"
#include "PEItemUseable.generated.h"

UCLASS()
class PROJECTESCAPE_API APEItemUseable : public APEItemBase, public IPEQuickSlotItem, public IPEUseable
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEItemUseable();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* Stat 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	TObjectPtr<UDataTable> EquipmentDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	FName EquipmentRowName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	FPEWeaponData EquipmentStats;

public:
	FPEWeaponData GetEquipmentStats();
	
	/* Quick Slot 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quick Slot")
	TObjectPtr<UPEQuickSlotItemComponent> QuickSlotItemComponent;
	
	/* IPEQuickSlotItem 인터페이스 선언 */
public:
	virtual AActor* GetItemOwner() const override;
	virtual void OnDropped(const FVector& Location, const FRotator& Rotation) override;
	virtual UPEQuickSlotItemComponent* GetQuickSlotItemComponent() const override;
	virtual EPEEquipmentType GetEquipmentType() const override;
	

	/* Useable 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Useable")
	TObjectPtr<UPEUseableComponent> UseableComponent;
	/* IPEUseable 인터페이스 선언 */
public:
	virtual void DoPrimaryAction(AActor* Holder) override;
	virtual void CompletePrimaryAction(AActor* Holder) override;
	virtual void DoSecondaryAction(AActor* Holder) override;
	virtual void DoTertiaryAction(AActor* Holder) override;
	virtual void OnHand(AActor* NewOwner) override;
	virtual void OnRelease() override;
	virtual UPEUseableComponent* GetUseableComponent() const override; 

	/* UI 반영 델리게이트 헬퍼 함수 관련 섹션 */
protected:
	FPEEquipmentInfo CreateCurrentEquipmentInfo() const;
	
	void BroadcastHUDStateChanged();
	void BroadcastEmptyHUDState();
	
	/* 아이템 장착 시각 효과 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	TObjectPtr<AActor> AttachedActor = nullptr;

	void AttachToOwner();
	void DetachFromOwner();
};
