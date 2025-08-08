// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/PEStorableItemComponent.h"

#include "Core/PELogChannels.h"
#include "Items/Interface/PEStorable.h"

UPEStorableItemComponent::UPEStorableItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UPEStorableItemComponent::BeginPlay()
{
	Super::BeginPlay();

	ComponentOwnerInterface = GetOwner();
}

void UPEStorableItemComponent::OnItemPickedUp() const
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->OnPickedUp();
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("PEStorableItemComponent: Owner %s does not implement IPEStorable interface!"), *GetOwner()->GetName());
	}
}

void UPEStorableItemComponent::OnItemDropped(const FVector& Location, const FRotator& Rotation) const
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->OnDropToWorld(Location, Rotation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PEStorableItemComponent: Owner %s does not implement IPEStorable interface!"), *GetOwner()->GetName());
	}
}

void UPEStorableItemComponent::AddItemCount(int32 Count) const
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->AddItemCount(Count);
	}
}

void UPEStorableItemComponent::OnItemDropped(int32 Count, const FVector& Location, const FRotator& Rotation) const
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->OnDropToWorld(Count, Location, Rotation);
	}
}

void UPEStorableItemComponent::ReduceItemCount(int32 Count)
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->ReduceItemCount(Count);
	}
}

void UPEStorableItemComponent::DestroyItem() const
{
	if (ComponentOwnerInterface)
	{
		ComponentOwnerInterface->DestoryItem();
	}
}

int32 UPEStorableItemComponent::GetItemCount() const
{
	if (ComponentOwnerInterface)
	{
		return ComponentOwnerInterface->GetItemCount();
	}
	return 0; // 기본값 반환
}

int32 UPEStorableItemComponent::GetStackCount() const
{
	if (ComponentOwnerInterface)
	{
		return ComponentOwnerInterface->GetItemStackCount();
	}
	return 0; // 기본값 반환
}


