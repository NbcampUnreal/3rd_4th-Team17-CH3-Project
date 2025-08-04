// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PEStorable.h"
#include "Interface/PEInteractable.h"
#include "PEItemBase.generated.h"

class UPEStorableItemComponent;
class UPEInteractableComponent;

UCLASS()
class PROJECTESCAPE_API APEItemBase : public AActor, public IPEInteractable, public IPEStorable
{
	GENERATED_BODY()
	
public:	
	APEItemBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	/* Interact 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UPEInteractableComponent> InteractableComponent;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<AActor> ItemOwnerActor;
	
public:
	virtual void Interact(AActor* Interactor) override;
	virtual bool IsInteractable() const override;

	/* Storable 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	TObjectPtr<UPEStorableItemComponent> StorableItemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
	int32 MaxStackCount;
	
	int32 ItemCount;
	int32 StackCount; // 아이템의 스택 개수(e.g. 최대 64개까지 스택 가능한 아이템이 90개 있을 경우 StackCount는 2가 됌)

	virtual void OnDuplicated();
public:
	virtual void OnPickedUp() override;
	virtual void OnDropToWorld(const FVector& Location, const FRotator& Rotation) override;
	virtual int32 GetItemCount() const override;
	virtual int32 GetItemStackCount() const override;
	virtual void AddItemCount(int32 Count) override;
	virtual void OnDropToWorld(int32 Count, const FVector& Location, const FRotator& Rotation) override;
	virtual void SplitAndDropItem(int32 Count, const FVector& Location, const FRotator& Rotation) override;
	virtual void DestoryItem() override;
};
