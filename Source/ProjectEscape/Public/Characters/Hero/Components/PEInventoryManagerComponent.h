// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PEInventoryManagerComponent.generated.h"

class UPEStorableItemComponent;
class APEHero;
struct FInventoryList;

/*
 *	인벤토리의 아이템을 관리하는 컴포넌트 클래스입니다.
 *	구현되어야 하는 기능:
 *	1. 아이템을 인벤토리에 추가하는 기능
 *	2. 인벤토리에서 아이템을 제거하는 기능
 *	3. 인벤토리에 있는 아이템을 반환하는 기능
 *	4. 인벤토리에 아이템이 있는지 확인하는 기능
 *	5. 인벤토리를 비우는 기능
 *	
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEInventoryManagerComponent();

protected:
	virtual void BeginPlay() override;

	/* Inventory 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, Category= "Inventory")
	TMap<FGameplayTag, TObjectPtr<UPEStorableItemComponent>> InventoryItems; 

	UPROPERTY(EditAnywhere, Category= "Inventory")
	int32 MaxInventorySize;

	UPROPERTY(VisibleAnywhere, Category= "Inventory")
	int32 CurrentItemInInventroyCount;
	
public:
	void AddItemToInventory(UPEStorableItemComponent* Item);
	void DropItemFromInventoryByTag(const int32 &Count, const FGameplayTag &Tag);
	void RemoveItemFromInventoryByTag(const FGameplayTag &Tag);
	void ClearInventory();
	UPEStorableItemComponent* GetItemByTag(const FGameplayTag &Tag) const;
	
protected:
	void SortInventory();
	void UpdateCurrentItemCount();
	

	FInventoryList ConvertToInventoryList() const;
	void BroadcastInventoryChanged();

public:
	void ItemDropTest();
};
