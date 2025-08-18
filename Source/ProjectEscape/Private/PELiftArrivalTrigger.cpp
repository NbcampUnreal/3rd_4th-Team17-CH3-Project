#include "PELiftArrivalTrigger.h"
#include "Components/BoxComponent.h"
#include "PEDoorActor.h"

APELiftArrivalTrigger::APELiftArrivalTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);

    // 캐빈이 겹치도록 Overlap 세팅 (필요 시 Preset 조정)
    Trigger->SetBoxExtent(FVector(150.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // DoorMesh가 WorldDynamic이면 OK
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

    // 엘리베이터로 지정한 그 문(캐빈)만 반응
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

