#include "Systems/Levels/TeleportPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"

TMap<TWeakObjectPtr<AActor>, double> ATeleportPad::LastTeleportStamp;

ATeleportPad::ATeleportPad()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root = Trigger
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    SetRootComponent(Trigger);
    Trigger->SetBoxExtent(FVector(80.f, 80.f, 80.f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Visual (optional)
    PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
    PadMesh->SetupAttachment(RootComponent);
    PadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Exit point
    ExitPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ExitPoint"));
    ExitPoint->SetupAttachment(RootComponent);
}

void ATeleportPad::BeginPlay()
{
    Super::BeginPlay();
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ATeleportPad::OnTriggerBegin);
}

void ATeleportPad::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep,
    const FHitResult& Sweep)
{
    if (!OtherActor) return;
    if (!Cast<APawn>(OtherActor)) return;                // Pawn�� �ڷ���Ʈ
    if (!TargetPad.IsValid() || TargetPad.Get() == this) return;
    if (!CanTeleport(OtherActor)) return;

    // ���� Ʈ������ ĸó
    const FTransform Dest = TargetPad->GetExitTransform();
    TWeakObjectPtr<AActor> WeakActor = OtherActor;

    // Chaos/Overlap ������ ����� ���� ���� ƽ���� �̷�
    GetWorldTimerManager().SetTimerForNextTick(
        FTimerDelegate::CreateWeakLambda(this, [this, WeakActor, Dest]()
            {
                if (!WeakActor.IsValid()) return;

                if (DoTeleport_Safe(WeakActor.Get(), Dest))
                {
                    // �պ� ����(���ʿ� Ÿ�ӽ�����)
                    MarkTeleported(WeakActor.Get());
                    if (TargetPad.IsValid()) TargetPad->MarkTeleported(WeakActor.Get());
                }
            })
    );
}

bool ATeleportPad::CanTeleport(AActor* Actor) const
{
    const double Now = GetWorld()->GetTimeSeconds();
    if (const double* Last = LastTeleportStamp.Find(Actor))
    {
        if (Now - *Last < ReenterCooldown)
            return false;
    }
    return true;
}

void ATeleportPad::MarkTeleported(AActor* Actor) const
{
    LastTeleportStamp.Add(Actor, GetWorld()->GetTimeSeconds());
}

FTransform ATeleportPad::GetExitTransform() const
{
    const FVector Loc = (ExitPoint ? ExitPoint->GetComponentLocation() : GetActorLocation()) + ArrivalOffset;
    const FRotator Rot = bMatchRotation
        ? (ExitPoint ? ExitPoint->GetComponentRotation() : GetActorRotation())
        : FRotator::ZeroRotator; // ȸ�� ���� ���ϸ� OldRot ����ϵ��� ȣ���/�ɼ� ����
    return FTransform(Rot, Loc);
}

bool ATeleportPad::DoTeleport_Safe(AActor* Actor, const FTransform& Dest)
{
    if (!IsValid(Actor) || Actor->IsActorBeingDestroyed() || Actor->GetWorld() != GetWorld())
        return false;

    const FRotator OldRot = Actor->GetActorRotation();
    const FRotator NewRot = bMatchRotation ? Dest.Rotator() : OldRot;
    const FVector  NewLoc = Dest.GetLocation();

    // ĳ����: ĸ�� �浹/�����Ʈ ��� OFF �� �̵� �� ����
    if (ACharacter* Char = Cast<ACharacter>(Actor))
    {
        UCapsuleComponent* Capsule = Char->GetCapsuleComponent();
        UCharacterMovementComponent* Move = Char->GetCharacterMovement();

        const FVector OldVel = Move ? Move->Velocity : FVector::ZeroVector;

        if (Move) { Move->StopMovementImmediately(); Move->DisableMovement(); }
        if (Capsule) Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        const bool bOK = Char->SetActorLocationAndRotation(
            NewLoc, NewRot, /*bSweep*/false, /*OutHit*/nullptr, ETeleportType::TeleportPhysics);

        if (Capsule) Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        if (Move)
        {
            Move->SetMovementMode(MOVE_Walking);
            if (bPreserveVelocity)
            {
                const FRotator Delta = bMatchRotation ? (NewRot - OldRot) : FRotator::ZeroRotator;
                Move->Velocity = Delta.RotateVector(OldVel) * VelocityScale;
            }
        }
        return bOK;
    }

    // ��-ĳ����: ��Ʈ�� ������Ƽ��� ���� �ù� OFF �� �̵� �� ON
    if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Actor->GetRootComponent()))
    {
        const bool bWasSim = Prim->IsSimulatingPhysics();
        if (bWasSim) Prim->SetSimulatePhysics(false);

        const bool bOK = Actor->SetActorLocationAndRotation(
            NewLoc, NewRot, /*bSweep*/false, /*OutHit*/nullptr, ETeleportType::TeleportPhysics);

        if (bWasSim) { Prim->SetSimulatePhysics(true); Prim->WakeAllRigidBodies(); }
        return bOK;
    }

    // �� ��
    return Actor->SetActorLocationAndRotation(
        NewLoc, NewRot, false, nullptr, ETeleportType::TeleportPhysics);
}