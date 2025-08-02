// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemBase.h"
#include "Items/Components/PEInteractableComponent.h"
#include "Items/Components/PEStorableItemComponent.h"

APEItemBase::APEItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// InteractableComponent 생성 및 설정
	InteractableComponent = CreateDefaultSubobject<UPEInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
	InteractableComponent->SetHiddenInGame(false);

	// Inventroy 관련 컴포넌트 생성 및 설정
	StorableItemComponent = CreateDefaultSubobject<UPEStorableItemComponent>(TEXT("StorableItemComponent"));
	
	ItemOwnerActor = nullptr;
	ItemCount = 1; // 기본 아이템 개수 설정
	StackCount = 1;
	MaxStackCount = 64;
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

void APEItemBase::OnPickedUp()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("APEItemBase::OnPickedUp called on %s"), *GetName());
}

void APEItemBase::OnDropToWorld(const FVector& Location, const FRotator& Rotation)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	UE_LOG(LogTemp, Warning, TEXT("APEItemBase::OnDropToWorld called on %s"), *GetName());
}

int32 APEItemBase::GetItemCount() const
{
	return ItemCount;
}

void APEItemBase::AddItemCount(int32 Count)
{
	ItemCount += Count;
	StackCount = 1 + (ItemCount / MaxStackCount);
}
