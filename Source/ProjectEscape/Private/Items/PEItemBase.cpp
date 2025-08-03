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
	ItemCount = 30; // 기본 아이템 개수 설정
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
	
	InteractableComponent->SetComponentOwnerInterface(this);
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

void APEItemBase::OnDuplicated()
{
	
}

void APEItemBase::OnPickedUp()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("APEItemBase::OnPickedUp called on %s"), *GetName());
}

void APEItemBase::OnDropToWorld(const FVector& Location, const FRotator& Rotation)
{
	SetActorLocation(Location);
	SetActorRotation(Rotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	ItemOwnerActor = nullptr;
	
	UE_LOG(LogTemp, Warning, TEXT("APEItemBase::OnDropToWorld called on %s"), *GetName());
}

int32 APEItemBase::GetItemCount() const
{
	return ItemCount;
}

int32 APEItemBase::GetItemStackCount() const
{
	return StackCount;
}

void APEItemBase::AddItemCount(int32 Count)
{
	ItemCount += Count;
	StackCount = 1 + (ItemCount / MaxStackCount);
}

void APEItemBase::ReduceItemCount(int32 Count, const FVector& Location, const FRotator& Rotation)
{
	if (Count >= ItemCount)
	{
		OnDropToWorld(Location, Rotation);
	}
	else
	{
		// 아이템 개수를 감소시킴
		ItemCount -= Count;
		StackCount = 1 + ((ItemCount - 1) / MaxStackCount);
		
		// 복제된 아이템을 생성
		if (GetWorld())
		{
			APEItemBase* DuplicatedItem = GetWorld()->SpawnActor<APEItemBase>(GetClass(), Location, Rotation);
			if (DuplicatedItem)
			{
				// 복제된 아이템의 속성을 설정
				DuplicatedItem->ItemCount = Count;
				DuplicatedItem->StackCount = 1 + ((Count - 1) / MaxStackCount);
				DuplicatedItem->MaxStackCount = MaxStackCount;
				DuplicatedItem->ItemOwnerActor = nullptr;
				
				// OnDuplicated 이벤트 호출
				DuplicatedItem->OnDuplicated();
				
				// 복제된 아이템을 월드에 드롭
				DuplicatedItem->OnDropToWorld(Location, Rotation);
				
				UE_LOG(LogTemp, Warning, TEXT("Item duplicated: Original count %d, Duplicated count %d"), ItemCount, Count);
			}
		}
	}
}

void APEItemBase::DestoryItem()
{
	//NOTE: Timer나 델리게이트 처럼 다른 곳에서 이 아이템을 참조하고 있다면,
	//DestoryItem() 호출 시점에 따라 문제가 발생할 수 있음
	if (GetWorld())
	{
		GetWorld()->DestroyActor(this);
		UE_LOG(LogTemp, Warning, TEXT("APEItemBase::DestoryItem called on %s"), *GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("APEItemBase::DestoryItem failed: World is null"));
	}
}
