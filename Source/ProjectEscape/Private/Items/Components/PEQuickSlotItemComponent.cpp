// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/PEQuickSlotItemComponent.h"

#include "Items/PEEquipmentType.h"
#include "Core/PELogChannels.h"
#include "Items/Interface/PEQuickSlotItem.h"

UPEQuickSlotItemComponent::UPEQuickSlotItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPEQuickSlotItemComponent::BeginPlay()
{
	Super::BeginPlay();

	ComponentOwnerActor = GetOwner();
	SetComponentInterface(ComponentOwnerActor);
}

void UPEQuickSlotItemComponent::OnItemPickedUp()
{
	if (ComponentOwnerActor)
	{
		ComponentOwnerActor->SetActorHiddenInGame(true);
		ComponentOwnerActor->SetActorEnableCollision(false);
	}
}

void UPEQuickSlotItemComponent::OnItemDropped()
{
	if (ComponentOwnerActor)
	{
		ComponentOwnerActor->SetActorHiddenInGame(false);
		ComponentOwnerActor->SetActorEnableCollision(true);
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("PEQuickSlotItemComponent: Owner actor is null!"));
	}
	
	if (ComponentOwnerActorInterface)
	{
		ComponentOwnerActorInterface->OnDropped();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PEQuickSlotItemComponent: Owner %s does not implement IPEQuickSlotItem interface!"), *ComponentOwnerActor->GetName());
	}
}

EPEEquipmentType UPEQuickSlotItemComponent::GetEquipmentType() const
{
	if (ComponentOwnerActorInterface)
	{
		return ComponentOwnerActorInterface->GetEquipmentType();
	}
	return EPEEquipmentType::None; // 기본값 반환
}

void UPEQuickSlotItemComponent::SetComponentInterface(UObject* NewOwner)
{
	
	if (NewOwner && NewOwner->Implements<UPEQuickSlotItem>())
	{
		ComponentOwnerActorInterface.SetObject(NewOwner);
		ComponentOwnerActorInterface.SetInterface(Cast<IPEQuickSlotItem>(NewOwner));
	}
	else
	{
		ComponentOwnerActorInterface.SetObject(nullptr);
		ComponentOwnerActorInterface.SetInterface(nullptr);
	}
}
