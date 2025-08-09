// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Hero/Components/PEUseableItemManagerComponent.h"

#include "Characters/Hero/PEHero.h"
#include "Characters/Hero/Components/PEQuickSlotManagerComponent.h"
#include "Core/PELogChannels.h"
#include "Items/Components/PEUseableComponent.h"


UPEUseableItemManagerComponent::UPEUseableItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentItemComponent = nullptr; // 초기에는 아이템이 설정되지 않음
}

void UPEUseableItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	ComponentOwnerActor = GetOwner();
}

void UPEUseableItemManagerComponent::SetHandItem(UPEUseableComponent* NewItemComponent)
{
	// 현재 손에 아이템이 있다면 먼저 해제
	if (CurrentItemComponent)
	{
		// 현재 아이템 해제 (필요한 경우 해제 로직 추가)
		CurrentItemComponent->Release();
		CurrentItemComponent = nullptr;

		UE_LOG(LogTemp, Log, TEXT("UPEUseableItemManagerComponent: Unequipping current item"));
	}

	// 새 아이템 장착
	CurrentItemComponent = NewItemComponent;
	if (CurrentItemComponent)
	{
		CurrentItemComponent->Hold();

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

void UPEUseableItemManagerComponent::ReleaseHandItem()
{
	if (CurrentItemComponent)
	{
		CurrentItemComponent = nullptr; // 현재 아이템 컴포넌트를 해제
	}
}

void UPEUseableItemManagerComponent::DropHandEquipmentToWorld()
{
	if (CurrentItemComponent && ComponentOwnerActor)
	{
		if (APEHero* Hero = Cast<APEHero>(ComponentOwnerActor))
		{
			FVector Location = ComponentOwnerActor->GetActorLocation();
			FRotator Rotation = ComponentOwnerActor->GetActorRotation();

			Hero->GetQuickSlotManagerComponent()->DropHandEquipmentToWorld(CurrentItemComponent->GetEquipmentType(), Location, Rotation);
			UE_LOG(LogPE, Log, TEXT("UPEUseableItemManagerComponent::DropHandEquipmentToWorld"));
		}
		ReleaseHandItem();
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("UPEUseableItemManagerComponent::DropHandEquipmentToWorld: CurrentItemComponent or ComponentOwnerActor is null!"));
	}
}

void UPEUseableItemManagerComponent::DoPrimaryActionCurrentItem(AActor* Holder)
{
	if (CurrentItemComponent)
	{
		CurrentItemComponent->DoPrimaryAction(Holder);
	}
}

void UPEUseableItemManagerComponent::CompletePrimaryActionCurrentItem(AActor* Holder)
{
	if (CurrentItemComponent)
	{
		CurrentItemComponent->CompletePrimaryAction(Holder);
	}
}

void UPEUseableItemManagerComponent::DoSecondaryActionCurrentItem(AActor* Holder)
{
	if (CurrentItemComponent)
	{
		CurrentItemComponent->DoSecondaryAction(Holder);
	}
}

void UPEUseableItemManagerComponent::DoTertiaryActionCurrentItem(AActor* Holder)
{
	if (CurrentItemComponent)
	{
		CurrentItemComponent->DoTertiaryAction(Holder);
	}
}
