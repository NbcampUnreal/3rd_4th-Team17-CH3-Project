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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEUseableComponent> HoldableComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEQuickSlotItemComponent> QuickSlotItemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> OwnerActor; // 아이템을 소유한 액터

	bool bIsInHand;
	
public:
	virtual void Interact(AActor* Interactor) override;
	virtual void Use(AActor* Holder) override;
	virtual void OnHand(AActor* NewOwner) override;
	virtual bool IsInteractable() const override;
	virtual UPEUseableComponent* GetUseableComponent() const override; // 사용 가능한 컴포넌트를 반환합니다

	/* Weapon Stat 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	EPEEquipmentType EquipmentType;

	/* IPEQuickSlotItem 인터페이스 선언 */
public:
	virtual AActor* GetItemOwner() const override;
	virtual UPEQuickSlotItemComponent* GetQuickSlotItemComponent() const override;
	virtual EPEEquipmentType GetEquipmentType() const override;
};

