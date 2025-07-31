// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemBase.h"
#include "Components/PEInteractableComponent.h"

APEItemBase::APEItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractableComponent = CreateDefaultSubobject<UPEInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
	InteractableComponent->SetHiddenInGame(false);

	ItemOwnerActor = nullptr;
}

void APEItemBase::BeginPlay()
{
	Super::BeginPlay();

}

void APEItemBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	InteractableComponent->SetComponentOwner(this);
}

void APEItemBase::Interact(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact called"));
	ItemOwnerActor = Interactor;
}

bool APEItemBase::IsInteractable() const
{
	if (ItemOwnerActor)
	{
		return false;
	}
	return true;
}

bool APEItemBase::CanBeStored() const
{
	return true;
}

bool APEItemBase::StoreItem(class AActor* Inventory)
{
	return true;
}

bool APEItemBase::RemoveFromStorage(class AActor* Inventory)
{
	return true;
}

int32 APEItemBase::GetStackSize() const
{
	return 0;
}

int32 APEItemBase::GetMaxStackSize() const
{
	return 0;
}

bool APEItemBase::AddToStack(int32 Amount)
{
	return true;
}

FString APEItemBase::GetItemID() const
{
	return FString(TEXT("DefaultItemID"));
}

bool APEItemBase::IsStackable() const
{
	return true;
}

bool APEItemBase::DropToWorld(const FVector& Location, const FRotator& Rotation)
{
	return true;
}
