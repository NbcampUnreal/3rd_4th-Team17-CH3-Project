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

    InteractableComponent = CreateDefaultSubobject<UPEInteractableComponent>(TEXT("InteractableComponent"));
    InteractableComponent->SetupAttachment(Mesh);
}

void APEDoorSwitch::Interact(AActor* Interactor)
{
    for (APEDoorActor* Door : LinkedDoors)
    {
        if (!IsValid(Door)) continue;

        switch (Command)
        {
        case ESwitchCommand::Toggle:     Door->ToggleOpen(); break;
        case ESwitchCommand::ForceOpen:  Door->Open();       break; // 2ÃþÀ¸·Î
        case ESwitchCommand::ForceClose: Door->Close();      break; // 1ÃþÀ¸·Î
        }
    }
}