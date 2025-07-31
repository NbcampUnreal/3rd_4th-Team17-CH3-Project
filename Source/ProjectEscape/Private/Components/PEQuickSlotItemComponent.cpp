// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PEQuickSlotItemComponent.h"

#include "PEEquipmentType.h"
#include "Interface/PEQuickSlotItem.h"

UPEQuickSlotItemComponent::UPEQuickSlotItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPEQuickSlotItemComponent::BeginPlay()
{
	Super::BeginPlay();

	ComponentOwnerActor = GetOwner();
}

void UPEQuickSlotItemComponent::OnItemPickedUp(AActor* NewItemOwner)
{
	if (ComponentOwnerActor)
	{
		// 아이템을 보이지 않게 설정
		ComponentOwnerActor->SetActorHiddenInGame(true);
		
		// 콜리전도 비활성화 (선택사항)
		ComponentOwnerActor->SetActorEnableCollision(false);
	}
}

void UPEQuickSlotItemComponent::OnItemDropped(AActor* NewItemOwner)
{
	if (ComponentOwnerActor)
	{
		// 아이템을 다시 보이게 설정
		ComponentOwnerActor->SetActorHiddenInGame(false);
		
		// 콜리전도 다시 활성화
		ComponentOwnerActor->SetActorEnableCollision(true);
	}
}

EPEEquipmentType UPEQuickSlotItemComponent::GetEquipmentType()
{
	if (ComponentOwnerActor)
	{
		if (IPEQuickSlotItem* QuickSlotItemInterface = Cast<IPEQuickSlotItem>(ComponentOwnerActor))
		{
			return QuickSlotItemInterface->GetEquipmentType();
		}
	}
	return EPEEquipmentType::None; // 기본값 반환
}
