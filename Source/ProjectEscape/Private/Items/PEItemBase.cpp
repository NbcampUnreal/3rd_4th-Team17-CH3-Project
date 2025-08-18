// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PEItemBase.h"

#include "Characters/Hero/Components/PEInventoryManagerComponent.h"
#include "Items/Components/PEInteractableComponent.h"
#include "Items/Components/PEStorableItemComponent.h"

APEItemBase::APEItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 컴포넌트 생성 및 설정
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	
	// InteractableComponent 생성 및 설정
	InteractableComponent = CreateDefaultSubobject<UPEInteractableComponent>(TEXT("InteractableComponent"));
	InteractableComponent->SetupAttachment(RootComponent);
	InteractableComponent->SetHiddenInGame(false);

	// Inventroy 관련 컴포넌트 생성 및 설정
	StorableItemComponent = CreateDefaultSubobject<UPEStorableItemComponent>(TEXT("StorableItemComponent"));
	
	ItemOwnerActor = nullptr;
	ItemCount = 5; // 기본 아이템 개수 설정
	StackCount = 1;
}

void APEItemBase::PostLoad()
{
	Super::PostLoad();
	InitializeFromDataTable();
}

void APEItemBase::BeginPlay()
{
	Super::BeginPlay();	
	StorableItemComponent->SetItemTag(ItemStats.ItemTag);
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

void APEItemBase::InitializeFromDataTable()
{
	if (ItemDataTable && !ItemRowName.IsNone())
	{
		if (FPEItemData* ItemData = ItemDataTable->FindRow<FPEItemData>(ItemRowName, TEXT("")))
		{
			ItemStats = *ItemData;
		}
	}
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

int32 APEItemBase::GetStackCapacity() const
{
	return ItemStats.StackCapacity;
}

FPEItemData APEItemBase::GetItemStats() const
{
	return ItemStats;
}

void APEItemBase::SetInventroyManagerComponent(UPEInventoryManagerComponent* NewComponentOwnerInterface)
{
	OwningInventoryManagerComponent = NewComponentOwnerInterface;
}

int32 APEItemBase::CalculateStackCount(int32 Count) const
{
	if (Count <= 0)
	{
		return 0;
	}
	return (Count - 1) / ItemStats.StackCapacity + 1;
}

void APEItemBase::AddItemCount(int32 Count)
{
	ItemCount += Count;
	StackCount = CalculateStackCount(ItemCount);
}

void APEItemBase::ReduceItemCount(int32 Count)
{
	ItemCount = FMath::Max(0, ItemCount - Count);

	if (ItemCount <= 0)
	{
		OwningInventoryManagerComponent->RemoveItemFromInventoryByTag(StorableItemComponent->GetItemTag());
		DestoryItem();
		return;
	}
	StackCount = CalculateStackCount(ItemCount);
}

void APEItemBase::OnDropToWorld(int32 Count, const FVector& Location, const FRotator& Rotation)
{
	if (Count >= ItemCount)
	{
		OnDropToWorld(Location, Rotation);
	}
	else
	{
		SplitAndDropItem(Count, Location, Rotation);
	}
}

void APEItemBase::SplitAndDropItem(int32 Count, const FVector& Location, const FRotator& Rotation)
{
	// 아이템 개수를 감소시킴
	ItemCount -= Count;
	StackCount = CalculateStackCount(ItemCount);
		
	// 복제된 아이템을 생성
	if (GetWorld())
	{
		if (APEItemBase* DuplicatedItem = GetWorld()->SpawnActor<APEItemBase>(GetClass(), Location, Rotation))
		{
			// 복제된 아이템의 속성을 설정
			DuplicatedItem->ItemCount = Count;
			DuplicatedItem->StackCount = CalculateStackCount(Count);
			DuplicatedItem->ItemRowName = ItemRowName;
			DuplicatedItem->ItemOwnerActor = nullptr;
			
			DuplicatedItem->OnDropToWorld(Location, Rotation);
				
			UE_LOG(LogTemp, Warning, TEXT("Item duplicated: Original count %d, Duplicated count %d"), ItemCount, Count);
		}
	}
}

void APEItemBase::DestoryItem()
{
	/*
	 *	NOTE: Timer나 델리게이트 처럼 다른 곳에서 이 아이템을 참조하고 있다면,
	 *	DestoryItem() 호출 시점에 따라 문제가 발생할 수 있음
	 */
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

void APEItemBase::OnUseFromInventory()
{
	// NOTE: 아이템을 인벤토리에서 사용했을 때의 액션은 이곳에 구현합니다.
	//			ItemBase는 기본적으로 아무 동작도 하지 않습니다.
	UE_LOG(LogTemp, Warning, TEXT("OnUseFromInventory called on %s"), *GetName());
}

