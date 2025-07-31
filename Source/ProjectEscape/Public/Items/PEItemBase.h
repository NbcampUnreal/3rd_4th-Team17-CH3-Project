// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PEStorable.h"
#include "Interface/PEInteractable.h"
#include "PEItemBase.generated.h"

class UPEInteractableComponent;

UCLASS()
class PROJECTESCAPE_API APEItemBase : public AActor, public IPEInteractable
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

	/* IPEStorable 인터페이스 선언 */
	virtual bool	CanBeStored() const;
	virtual bool	StoreItem(class AActor* Inventory);
	virtual bool	RemoveFromStorage(class AActor* Inventory);
	virtual int32	GetStackSize() const;
	virtual int32	GetMaxStackSize() const;
	virtual bool	AddToStack(int32 Amount);
	virtual FString GetItemID() const;
	virtual bool	IsStackable() const;
	virtual bool	DropToWorld(const FVector& Location, const FRotator& Rotation);
};
