#include "Characters/Hero/Components/PEHeroInputComponent.h"

UPEHeroInputComponent::UPEHeroInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPEHeroInputComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UPEHeroInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

