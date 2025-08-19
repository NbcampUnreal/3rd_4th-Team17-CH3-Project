#include "PELiftArrivalTrigger.h"
#include "Components/BoxComponent.h"
#include "PEDoorActor.h"

APELiftArrivalTrigger::APELiftArrivalTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);

    // ĳ���� ��ġ���� Overlap ���� (�ʿ� �� Preset ����)
    Trigger->SetBoxExtent(FVector(150.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // DoorMesh�� WorldDynamic�̸� OK
    Trigger->SetGenerateOverlapEvents(true);
}

void APELiftArrivalTrigger::BeginPlay()
{
    Super::BeginPlay();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &APELiftArrivalTrigger::OnTriggerBegin);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &APELiftArrivalTrigger::OnTriggerEnd);
}

void APELiftArrivalTrigger::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex,
    bool bFromSweep, const FHitResult& Sweep)
{
    if (!IsValid(ElevatorCab) || bFiredOnce) return;

    // ���������ͷ� ������ �� ��(ĳ��)�� ����
    if (OtherActor == ElevatorCab)
    {
        OpenAll();
        if (bOneShot)
        {
            bFiredOnce = true;
            Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            Trigger->SetGenerateOverlapEvents(false);
        }
    }
}

void APELiftArrivalTrigger::OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
    if (!IsValid(ElevatorCab) || !bCloseOnLeave) return;
    if (OtherActor == ElevatorCab)
    {
        CloseAll();
    }
}

void APELiftArrivalTrigger::OpenAll()
{
    for (APEDoorActor* Door : EntranceDoors)
    {
        if (IsValid(Door)) Door->Open();
    }
}

void APELiftArrivalTrigger::CloseAll()
{
    for (APEDoorActor* Door : EntranceDoors)
    {
        if (IsValid(Door)) Door->Close();
    }
}

