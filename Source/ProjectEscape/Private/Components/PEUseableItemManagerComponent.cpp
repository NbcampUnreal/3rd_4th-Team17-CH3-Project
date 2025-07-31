// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PEUseableItemManagerComponent.h"
#include "Interface/PEUseable.h"
#include "Components/PEUseableComponent.h"


UPEUseableItemManagerComponent::UPEUseableItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentItemComponent = nullptr; // 초기에는 아이템이 설정되지 않음
}

void UPEUseableItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
}

/*
void UPEUseableItemManagerComponent::SetHandItem(IPEUseable* NewItem)
{
	// IPEUseable 인터페이스를 UPEUseableComponent로 캐스팅하여 설정
	if (NewItem)
	{
		CurrentItemComponent = Cast<UPEUseableComponent>(NewItem);
		if (CurrentItemComponent)
		{
			UE_LOG(LogTemp, Log, TEXT("UPEUseableItemManagerComponent: Set current item to %s"), 
				*CurrentItemComponent->GetOwner()->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UPEUseableItemManagerComponent: Failed to cast IPEUseable to UPEUseableComponent"));
		}
	}
	else
	{
		CurrentItemComponent = nullptr;
		UE_LOG(LogTemp, Log, TEXT("UPEUseableItemManagerComponent: Cleared current item"));
	}
}
}
*/

void UPEUseableItemManagerComponent::SetHandItem(UPEUseableComponent* NewItemComponent)
{
	// 현재 손에 아이템이 있다면 먼저 해제
	if (CurrentItemComponent)
	{
		// 현재 아이템 해제 (필요한 경우 해제 로직 추가)
		CurrentItemComponent->Release(OwnerActor);
		CurrentItemComponent = nullptr;
		
		UE_LOG(LogTemp, Log, TEXT("UPEUseableItemManagerComponent: Unequipping current item"));
	}

	// 새 아이템 장착
	CurrentItemComponent = NewItemComponent;
	if (CurrentItemComponent)
	{
		CurrentItemComponent->Hold(OwnerActor);
		
		UE_LOG(LogTemp, Log, TEXT("UPEUseableItemManagerComponent: Set current item component to %s"), 
			*CurrentItemComponent->GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("UPEUseableItemManagerComponent: Cleared current item component"));
	}
	// Todo: 아이템 변경 애니메이션 실행
}

UPEUseableComponent* UPEUseableItemManagerComponent::GetCurrentItem() const
{
	return CurrentItemComponent;
}

void UPEUseableItemManagerComponent::UseCurrentItem(AActor* Holder)
{
	if (CurrentItemComponent)
	{
		CurrentItemComponent->Use(Holder);
	}
}
