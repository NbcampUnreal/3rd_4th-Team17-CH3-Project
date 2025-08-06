#include "Characters/Hero/PEHero.h"
#include "Characters/Hero/Components/PEHeroInputComponent.h"

APEHero::APEHero()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Create Components */
	HeroInputComponent = CreateDefaultSubobject<UPEHeroInputComponent>("InputComponent");
}

void APEHero::BeginPlay()
{
	Super::BeginPlay();
	
}

void APEHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APEHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

