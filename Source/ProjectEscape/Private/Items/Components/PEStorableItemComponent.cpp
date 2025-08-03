// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/PEStorableItemComponent.h"

#include "Items/Interface/PEStorable.h"

UPEStorableItemComponent::UPEStorableItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UPEStorableItemComponent::BeginPlay()
{
	Super::BeginPlay();

	ComponentOwnerActor = GetOwner();
	ComponentOwnerInterface = GetOwner();
}

void UPEStorableItemComponent::OnItemPickedUp() const
{
	if (ComponentOwnerActor)
	{
		if (IPEStorable* StorableInterface = Cast<IPEStorable>(ComponentOwnerActor))
		{
			StorableInterface->OnPickedUp();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEStorableItemComponent: Owner %s does not implement IPEStorable interface!"), *ComponentOwnerActor->GetName());
		}
	}
}

void UPEStorableItemComponent::OnItemDropped(int32 Count, const FVector& Location, const FRotator& Rotation) const
{
	if (ComponentOwnerActor)
	{
		if (IPEStorable* StorableInterface = Cast<IPEStorable>(ComponentOwnerActor))
		{
			StorableInterface->OnDropToWorld(Location, Rotation);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEStorableItemComponent: Owner %s does not implement IPEStorable interface!"), *ComponentOwnerActor->GetName());
		}
	}
}

void UPEStorableItemComponent::AddItemCount(int32 Count) const
{
	if (IPEStorable* StorableInterface = Cast<IPEStorable>(ComponentOwnerActor))
	{
		StorableInterface->AddItemCount(Count);
	}
}

void UPEStorableItemComponent::ReduceItemCount(int32 Count, const FVector& Location, const FRotator& Rotation) const
{
	if (IPEStorable* StorableInterface = Cast<IPEStorable>(ComponentOwnerActor))
	{
		StorableInterface->ReduceItemCount(Count, Location, Rotation);
	}
}

void UPEStorableItemComponent::DestoryItem() const
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->DestoryItem();
	}
}

int32 UPEStorableItemComponent::GetItemCount() const
{
	if (IPEStorable* StorableInterface = Cast<IPEStorable>(ComponentOwnerActor))
	{
		return StorableInterface->GetItemCount();
	}
	return 0; // 기본값 반환
}

int32 UPEStorableItemComponent::GetStackCount() const
{
	if (IPEStorable* StorableInterface = Cast<IPEStorable>(ComponentOwnerActor))
	{
		return StorableInterface->GetItemStackCount();
	}
	return 0; // 기본값 반환
	
}


