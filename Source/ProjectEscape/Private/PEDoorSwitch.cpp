#include "PEDoorSwitch.h"
#include "PEDoorActor.h"
#include "Components/StaticMeshComponent.h"

APEDoorSwitch::APEDoorSwitch()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);
}

void APEDoorSwitch::Interact(AActor* Interactor)
{
    for (APEDoorActor* Door : LinkedDoors)
    {
        if (IsValid(Door))
        {
            Door->ToggleOpen();
        }
    }
}