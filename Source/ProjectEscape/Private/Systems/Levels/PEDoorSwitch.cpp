#include "Systems/Levels/PEDoorSwitch.h"
#include "Systems/Levels/PEDoorActor.h"
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

    InteractWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionUIComponent"));
    InteractWidgetComponent->SetupAttachment(RootComponent);
    InteractWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, InteractionUIOffsetZ)); // ������ ���ʿ� ǥ��
    InteractWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 3D ������ ǥ��
    InteractWidgetComponent->SetDrawSize(FVector2D(200.0f, 100.0f)); // UI ũ�� ����
    InteractWidgetComponent->SetVisibility(false); // �⺻������ ����
}

void APEDoorSwitch::Interact(AActor* Interactor)
{
    for (APEDoorActor* Door : LinkedDoors)
    {
        if (!IsValid(Door)) continue;

        switch (Command)
        {
        case ESwitchCommand::Toggle:     Door->ToggleOpen(); break;
        case ESwitchCommand::ForceOpen:  Door->Open();       break; // 2������
        case ESwitchCommand::ForceClose: Door->Close();      break; // 1������
        }
    }
}

void APEDoorSwitch::ShowInteractionUI()
{
    if (InteractWidgetComponent && InteractWidgetClass)
    {
        // ���� Ŭ������ �����Ǿ� ������ ���� ����
        if (!InteractWidgetComponent->GetWidget())
        {
            InteractWidgetComponent->SetWidgetClass(InteractWidgetClass);
        }

        InteractWidgetComponent->SetVisibility(true);
        UE_LOG(LogTemp, Log, TEXT("Interaction UI shown for %s"), *GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("InteractionUIComponent or InteractionUIWidgetClass is null for %s"), *GetName());
    }
}

void APEDoorSwitch::HideInteractionUI()
{
    if (InteractWidgetComponent)
    {
        InteractWidgetComponent->SetVisibility(false);
        UE_LOG(LogTemp, Log, TEXT("Interaction UI hidden for %s"), *GetName());
    }
}
