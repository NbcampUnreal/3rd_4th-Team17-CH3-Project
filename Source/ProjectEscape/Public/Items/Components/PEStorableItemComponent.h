// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Core/PEGameplayTags.h"
#include "PEStorableItemComponent.generated.h"

/*
 *	인벤토리에 보관 가능한 아이템 또는 장비에 대한 컴포넌트 클래스입니다.
 *	구현해야 하는 기능:
 *	1. 아이템 태그 설정 및 반환 기능
 *	2. 아이템을 주울 때 호출되는 함수
 *	3. 아이템을 떨어뜨릴 때 호출되는 함수
 *	
 */

class UPEStorable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEStorableItemComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEStorableItemComponent();

protected:
	virtual void BeginPlay() override;

	/* Storable Item 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<AActor> ComponentOwnerActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (Categories = "Item"))
	FGameplayTag ItemTag;

public:
	FGameplayTag GetItemTag() const { return ItemTag; }
	void SetItemTag(const FGameplayTag& NewTag) { ItemTag = NewTag; }

	void OnItemPickedUp() const;
	void OnItemDropped(int32 Count, const FVector& Location, const FRotator& Rotation) const;
	void AddItemCount(int32 Count) const;
	void ReduceItemCount(int32 Count, const FVector& Location, const FRotator& Rotation) const;
	int32 GetItemCount() const;
	int32 GetStackCount() const;
};
