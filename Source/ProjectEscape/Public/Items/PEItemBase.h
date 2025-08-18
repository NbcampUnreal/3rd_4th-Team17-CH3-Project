// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PEStorable.h"
#include "Interface/PEInteractable.h"
#include "Engine/DataTable.h"
#include "Items/FPEItemData.h"
#include "PEItemBase.generated.h"

class UWidgetComponent;
class UPEStorableItemComponent;
class UPEInteractableComponent;

UCLASS()
class PROJECTESCAPE_API APEItemBase : public AActor, public IPEInteractable, public IPEStorable
{
	GENERATED_BODY()
	
public:	
	APEItemBase();

protected:
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/* Mesh 관련 섹션 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	/* Interact 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEInteractableComponent> InteractableComponent;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<AActor> ItemOwnerActor;
	
public:
	virtual void Interact(AActor* Interactor) override;
	virtual bool IsInteractable() const override;

	// 3D UI Widget 관련
public:
	virtual void ShowInteractionUI() override;
	virtual void HideInteractionUI() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> InteractWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> InteractWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float InteractionUIOffsetZ = 10.0f; 
	

	/* Storable 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	TObjectPtr<UPEStorableItemComponent> StorableItemComponent;

	UPROPERTY(VisibleAnywhere, Category = "Storage")
	TWeakObjectPtr<UPEInventoryManagerComponent> OwningInventoryManagerComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	int32 ItemCount;
	
	int32 StackCount; // 아이템의 스택 개수(e.g. 최대 64개까지 스택 가능한 아이템이 90개 있을 경우 StackCount는 2가 됌)

	/* DataTable 관련 섹션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName ItemRowName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Data")
	FPEItemData ItemStats;
	
	int32 CalculateStackCount(int32 Count) const;

	void InitializeFromDataTable();

public:
	virtual void OnPickedUp() override;
	virtual void OnDropToWorld(const FVector& Location, const FRotator& Rotation) override;
	virtual void OnDropToWorld(int32 Count, const FVector& Location, const FRotator& Rotation) override;
	virtual void SplitAndDropItem(int32 Count, const FVector& Location, const FRotator& Rotation) override;
	virtual int32 GetItemCount() const override;
	virtual int32 GetItemStackCount() const override;
	virtual int32 GetStackCapacity() const override;
	virtual FPEItemData GetItemStats() const override;
	virtual void SetInventroyManagerComponent(UPEInventoryManagerComponent* NewComponentOwnerInterface) override;
	virtual void AddItemCount(int32 Count) override;
	virtual void ReduceItemCount(int32 Count) override;
	virtual void DestoryItem() override;

	/* UI 관련 섹션 */
	virtual void OnUseFromInventory();
};
