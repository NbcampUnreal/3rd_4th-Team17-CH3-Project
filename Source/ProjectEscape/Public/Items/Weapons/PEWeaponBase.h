// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PEQuickSlotItem.h"
#include "Interface/PEInteractable.h"
#include "Interface/PEUseable.h"
#include "PEWeaponBase.generated.h"

class UPEQuickSlotItemComponent;
enum class EPEEquipmentType : uint8;
class UPEUseableComponent;
class UPEInteractableComponent;

UCLASS()
class PROJECTESCAPE_API APEWeaponBase : public AActor, public IPEInteractable, public IPEUseable, public IPEQuickSlotItem
{
	GENERATED_BODY()
	
public:	
	APEWeaponBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/* Interact 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEInteractableComponent> InteractableComponent;
	
	/* IPEInteractable 인터페이스 선언 */
public:
	virtual void Interact(AActor* Interactor) override;
	virtual bool IsInteractable() const override;

	/* Weapon Stat 관련 섹션 */
	//Todo: 무기 타입을 Tag로 관리하도록 변경
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	EPEEquipmentType EquipmentType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> OwnerActor; // 아이템을 소유한 액터
	
	bool bIsInHand;

	/* 퀵슬롯 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEQuickSlotItemComponent> QuickSlotItemComponent;
	
	/* IPEQuickSlotItem 인터페이스 선언 */
public:
	virtual AActor* GetItemOwner() const override;
	virtual void OnDropped() override;
	virtual UPEQuickSlotItemComponent* GetQuickSlotItemComponent() const override;
	virtual EPEEquipmentType GetEquipmentType() const override;

	/* 아이템 사용 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEUseableComponent> HoldableComponent;

	/* IPEUseable 인터페이스 선언 */
public:
	virtual void Use(AActor* Holder) override;
	virtual void OnHand(AActor* NewOwner) override;
	virtual UPEUseableComponent* GetUseableComponent() const override; // 사용 가능한 컴포넌트를 반환합니다
};

