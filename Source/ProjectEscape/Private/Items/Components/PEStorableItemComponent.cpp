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
	
}

void UPEStorableItemComponent::OnItemPickedUp()
{
	if (ComponentOwnerActor)
	{
		// 아이템을 보이지 않게 설정
		ComponentOwnerActor->SetActorHiddenInGame(true);
		
		// 콜리전도 비활성화 (선택사항)
		ComponentOwnerActor->SetActorEnableCollision(false);
		
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

void UPEStorableItemComponent::OnItemDropped(const FVector& Location, const FRotator& Rotation)
{
	if (ComponentOwnerActor)
	{
		// 아이템을 다시 보이게 설정
		ComponentOwnerActor->SetActorHiddenInGame(false);
		
		// 콜리전도 다시 활성화
		ComponentOwnerActor->SetActorEnableCollision(true);
		
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


