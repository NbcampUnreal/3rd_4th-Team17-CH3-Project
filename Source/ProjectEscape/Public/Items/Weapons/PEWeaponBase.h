// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPEWeaponData.h"
#include "GameFramework/Actor.h"
#include "Items/Components/PEStorableItemComponent.h"
#include "Items/Interface/PEQuickSlotItem.h"
#include "Items/Interface/PEInteractable.h"
#include "Items/Interface/PEUseable.h"
#include "PEWeaponBase.generated.h"

struct FPEWeaponData;
class UPEAttackBaseComponent;
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

	/* Mesh 관련 섹션 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	
	/* Weapon Stat 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	TObjectPtr<UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
	FName WeaponRowName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	FPEWeaponData WeaponStats;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	TObjectPtr<AActor> WeaponOwnerActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	TWeakObjectPtr<UPEStorableItemComponent> AmmoComponent; // 소모되는 탄약으로 선택된 액터의 컴포넌트
	
	UPROPERTY()
	FTimerHandle ReloadTimerHandle;
	
	bool bIsFiring;
	bool bIsReloading;
	int32 CurrentAmmoCount;
	
public:
	bool TryReload();
	void CancleReload();

protected:
	void PerformReload();

	/* Interact 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEInteractableComponent> InteractableComponent;
	
	/* IPEInteractable 인터페이스 선언 */
public:
	virtual void Interact(AActor* Interactor) override;
	virtual bool IsInteractable() const override;
	
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
	virtual void DoPrimaryAction(AActor* Holder) override;
	virtual void CompletePrimaryAction(AActor* Holder) override;
	virtual void DoSecondaryAction(AActor* Holder) override;
	virtual void DoTertiaryAction(AActor* Holder) override;
	virtual void OnHand(AActor* NewOwner) override;
	virtual void OnRelease(AActor* NewOwner) override;
	virtual UPEUseableComponent* GetUseableComponent() const override;

	/* Combat(Attack) 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Hitscan")
	TObjectPtr<UPEAttackBaseComponent> AttackComponent;

	UPROPERTY()
	float LastAttackTime;

	virtual UPEAttackBaseComponent* CreateAttackComponent();
};
