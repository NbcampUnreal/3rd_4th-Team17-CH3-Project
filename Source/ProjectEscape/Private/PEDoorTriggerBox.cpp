#include "PEDoorTriggerBox.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "PEDoorActor.h"

APEDoorTriggerBox::APEDoorTriggerBox()
{
    PrimaryActorTick.bCanEverTick = false;

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);

    // �⺻ �浹 ����: Pawn Overlap�� �ѱ�
    Trigger->SetBoxExtent(FVector(120.f, 120.f, 120.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APEDoorTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    Trigger->OnComponentBeginOverlap.AddDynamic(this, &APEDoorTriggerBox::OnTriggerBegin);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &APEDoorTriggerBox::OnTriggerEnd);
}

void APEDoorTriggerBox::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep,
    const FHitResult& Sweep)
{
    if (!OtherActor) return;

    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;

    if (bOnlyAffectPlayer0)
    {
        APawn* Player0 = UGameplayStatics::GetPlayerPawn(this, 0);
        if (Pawn != Player0) return;
    }

    // ���ο� ���� �� ī��Ʈ ����
    OverlapCount = FMath::Max(0, OverlapCount) + 1;

    // ���� �ݱ� ������ �־����� ���
    CancelScheduledClose();

    if (bOpenOnEnter)
    {
        OpenAll();
    }
}

void APEDoorTriggerBox::OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
    if (!OtherActor) return;

    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;

    if (bOnlyAffectPlayer0)
    {
        APawn* Player0 = UGameplayStatics::GetPlayerPawn(this, 0);
        if (Pawn != Player0) return;
    }

    // ���� �� ī��Ʈ ����
    OverlapCount = FMath::Max(0, OverlapCount - 1);

    if (OverlapCount == 0)
    {
        if (CloseDelay > 0.f)
        {
            ScheduleClose();
        }
        else if (bCloseOnExit)
        {
            CloseAll();
        }
    }
}

void APEDoorTriggerBox::OpenAll()
{
    for (APEDoorActor* Door : LinkedDoors)
    {
        if (IsValid(Door)) Door->Open();
    }
}

void APEDoorTriggerBox::CloseAll()
{
    for (APEDoorActor* Door : LinkedDoors)
    {
        if (IsValid(Door)) Door->Close();
    }
}

void APEDoorTriggerBox::ScheduleClose()
{
    CancelScheduledClose();
    GetWorldTimerManager().SetTimer(
        CloseTimerHandle,
        FTimerDelegate::CreateWeakLambda(this, [this]()
            {
                // ������ �ٽ� �������� ���� ����
                if (OverlapCount == 0)
                {
                    CloseAll();
                }
            }),
        CloseDelay,
        false
    );
}

void APEDoorTriggerBox::CancelScheduledClose()
{
    if (CloseTimerHandle.IsValid())
    {
        GetWorldTimerManager().ClearTimer(CloseTimerHandle);
    }
}


