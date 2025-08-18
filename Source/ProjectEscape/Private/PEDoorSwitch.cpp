//#include "Actors/PEDoorSwitch.h"
#include "Actors/PEDoorActor.h"

APEDoorSwitch::APEDoorSwitch()
{
    bReplicates = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Root);
    Mesh->SetIsReplicated(true);
}

void APEDoorSwitch::Interact_Implementation(APawn* InstigatorPawn)
{
    if (HasAuthority())
    {
        Server_ToggleLinked(); // 서버에서 실행
    }
    else
    {
        Server_ToggleLinked(); // 클라 → 서버 RPC
    }
}

void APEDoorSwitch::Server_ToggleLinked_Implementation()
{
    for (APEDoorSlidingActor* Door : LinkedDoors)
    {
        if (IsValid(Door))
        {
            Door->ToggleOpen();
        }
    }
}