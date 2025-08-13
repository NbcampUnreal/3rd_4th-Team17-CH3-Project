// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "UI/Inventory/PEInventoryType.h"
#include "PEQuickSlotManagerComponent.generated.h"


class IPEUseable;
enum class EPEEquipmentType : uint8;

/*
 *	퀵슬롯을 관리하는 컴포넌트 클래스입니다.
 *	이 컴포넌트는 액터에 부착되어 퀵슬롯 아이템을 관리합니다.
 *	구현해야 하는 기능:
 *	1. 퀵슬롯 아이템을 설정하는 기능
 *	2. 퀵슬롯 아이템을 제거하는 기능
 *	3. 퀵슬롯 아이템을 선택하는 기능
 *	4. 퀵슬롯 아이템을 초기화하는 기능
 *	
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEQuickSlotManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEQuickSlotManagerComponent();

protected:
	virtual void BeginPlay() override;

	/* Quick Slot 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quick Slot")
	TMap<EPEEquipmentType, TObjectPtr<AActor>> QuickSlotItems;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quick Slot")
	TObjectPtr<AActor> EquipOwner;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void SetQuickSlotItem(EPEEquipmentType EquipmentType, AActor* ItemActor);
	
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void RemoveQuickSlotItem(EPEEquipmentType EquipmentType);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	AActor* SelectEquipment(EPEEquipmentType EquipmentType);
	
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void ClearQuickSlots();
	
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	bool ContainWeaponType(EPEEquipmentType EquipmentType);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void DropHandEquipmentToWorld(EPEEquipmentType EquipmentType, const FVector& Location, const FRotator& Rotation);

	AActor* GetActorFromQuickSlot(EPEEquipmentType EquipmentType) const;
	TMap<EPEEquipmentType, TObjectPtr<AActor>> GetQuickSlotItem(EPEEquipmentType EquipmentType) const;

protected:
	FInventoryInfo ConvertToQuickSlotInfo();

	/* UI 반영 델리게이트 헬퍼 함수 관련 섹션*/
protected:
	void UpdateQuickSlotInfoAndBroadcast();
};
