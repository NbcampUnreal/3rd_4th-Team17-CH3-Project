// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "PEInventoryManagerComponent.generated.h"

class UPEStorableItemComponent;
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
	void RemoveItemFromInventory(int32 Count, UPEStorableItemComponent* Item);
	bool IsItemInInventory(UPEStorableItemComponent* Item) const;
	bool IsItemInInventoryByTag(const FGameplayTag &Tag) const;
	void ClearInventory();
	UPEStorableItemComponent* GetItemByTag(const FGameplayTag &Tag);
protected:
	//void AddItemStack(); // 동일한 아이템이 있는 경우 스택 추가
	void SortInventory();
	void UpdateCurrentItemCount();

public:
	void ItemDropTest(); // 아이템 버리기 테스트용 코드
};
