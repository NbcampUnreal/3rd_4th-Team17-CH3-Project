// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PEUseableComponent.h"

#include "Interface/PEUseable.h"


// Sets default values for this component's properties
UPEUseableComponent::UPEUseableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentHolder = nullptr;
	bIsHold = false;
}

void UPEUseableComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UPEUseableComponent::Hold()
{
	bIsHold = true;
}

void UPEUseableComponent::Release()
{
	bIsHold = false;
}

void UPEUseableComponent::Use(AActor* User)
{
	UE_LOG(LogTemp, Warning, TEXT("UPEUseableComponent::Use"));
	if (bIsHold)
	{
		if (IPEUseable* UseableInterface = Cast<IPEUseable>(GetOwner()))
		{
			UseableInterface->Use(User);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEUseableComponent: Owner %s does not implement IPEUseable interface!"), *GetOwner()->GetName());
		}
	}
}