// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/PEUseableComponent.h"

#include "Core/PELogChannels.h"
#include "Items/Interface/PEQuickSlotItem.h"
#include "Items/Interface/PEUseable.h"


// Sets default values for this component's properties
UPEUseableComponent::UPEUseableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bIsHold = false;
}

void UPEUseableComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPEUseableComponent::Hold()
{
	if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
	{
		UseableInterface->OnHand(GetOwner());
		bIsHold = true;
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
	}
	UE_LOG(LogPE, Log, TEXT("UPEUseableComponent::Hold called on %s"), *GetOwner()->GetName());
}

void UPEUseableComponent::Release()
{
	if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
	{
		UseableInterface->OnRelease();
		bIsHold = false;
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
	}
	UE_LOG(LogPE, Log, TEXT("UPEUseableComponent::Hold called on %s"), *GetOwner()->GetName());

}

void UPEUseableComponent::DoPrimaryAction(AActor* User)
{
	if (bIsHold)
	{
		if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
		{
			UseableInterface->DoPrimaryAction(User);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
		}
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("UPEUseableComponent::DoPrimaryAction called but component is not held!"));
	}
}

void UPEUseableComponent::CompletePrimaryAction(AActor* User)
{
	UE_LOG(LogPE, Warning, TEXT("UPEUseableComponent::CompletePrimaryAction"));
	if (bIsHold)
	{
		if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
		{
			UseableInterface->CompletePrimaryAction(User);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
		}
	}
}

void UPEUseableComponent::DoSecondaryAction(AActor* User)
{
	UE_LOG(LogPE, Warning, TEXT("UPEUseableComponent::DoSecondaryAction"));
	if (bIsHold)
	{
		if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
		{
			UseableInterface->DoSecondaryAction(User);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
		}
	}
}

void UPEUseableComponent::DoTertiaryAction(AActor* User)
{
	UE_LOG(LogPE, Warning, TEXT("UPEUseableComponent::DoTertiaryAction"));
	if (bIsHold)
	{
		if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
		{
			UseableInterface->DoTertiaryAction(User);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
		}
	}
}

bool UPEUseableComponent::IsHolding() const
{
	return bIsHold;
}

EPEEquipmentType UPEUseableComponent::GetEquipmentType() const
{
	if (IPEQuickSlotItem* QuickSlotItemInterface = Cast<IPEQuickSlotItem>(GetOwner()))
	{
		return QuickSlotItemInterface->GetEquipmentType();
	}
	UE_LOG(LogPE, Warning, TEXT("UPEUseableComponent::GetEquipmentType: Owner %s does not implement IPEQuickSlotItem interface!"), *GetOwner()->GetName());
	return EPEEquipmentType::None; // 기본값 반환
}
