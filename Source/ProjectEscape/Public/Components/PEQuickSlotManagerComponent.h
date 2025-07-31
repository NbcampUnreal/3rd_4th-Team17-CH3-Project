// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEQuickSlotManagerComponent.generated.h"


class IPEUseable;
enum class EPEEquipmentType : uint8;

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
	
	TMap<EPEEquipmentType, TObjectPtr<AActor>> GetQuickSlotItem(EPEEquipmentType EquipmentType) const;
	
};
