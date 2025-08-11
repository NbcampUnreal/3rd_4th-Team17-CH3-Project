// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/PEInteractableComponent.h"
#include "Items/Interface/PEInteractable.h"

UPEInteractableComponent::UPEInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ComponentOwner = nullptr;
}

void UPEInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 소유자 액터가 IPEInteractable 인터페이스를 구현하는지 확인
	SetComponentOwnerInterface(GetOwner());
}

bool UPEInteractableComponent::Interact(AActor* Interactor)
{
	if (!ComponentOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("PEInteractableComponent: ComponentOwner is null!"));
		return false;
	}
	
	if (Interactor)
	{
		if (!ComponentOwner->IsInteractable())
		{
			return false;
		}
		ComponentOwner->Interact(Interactor);
		UE_LOG(LogTemp, Log, TEXT("Interact with %s by %s"), *GetOwner()->GetName(), *Interactor->GetName());

	}
	return true;
}

void UPEInteractableComponent::SetComponentOwnerInterface(UObject* NewOwner)
{
	if (NewOwner && NewOwner->Implements<UPEInteractable>())
	{
		ComponentOwner.SetObject(NewOwner);
		ComponentOwner.SetInterface(Cast<IPEInteractable>(NewOwner));
	}
	else
	{
		ComponentOwner.SetObject(nullptr);
		ComponentOwner.SetInterface(nullptr);
	}
}
