#include "PEDoorActor.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
APEDoorActor::APEDoorActor()
{
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(Root);
    DoorMesh->SetIsReplicated(true);
}

void APEDoorActor::BeginPlay()
{
    Super::BeginPlay();
    // 닫힘 기준 위치 고정(드리프트 방지)
    BaseLoc = DoorMesh->GetRelativeLocation();
}

void APEDoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APEDoorActor, bOpen);
}

FVector APEDoorActor::DirVectorLocal() const
{
    switch (Direction)
    {
    case ESlideDirection::LocalX_Pos: return FVector(+1, 0, 0);
    case ESlideDirection::LocalX_Neg: return FVector(-1, 0, 0);
    case ESlideDirection::LocalY_Pos: return FVector(0, +1, 0);
    case ESlideDirection::LocalY_Neg: return FVector(0, -1, 0);
    case ESlideDirection::LocalZ_Pos: return FVector(0, 0, +1);
    default:                          return FVector(0, 0, -1);
    }
}

void APEDoorActor::Open()
{
    if (!bOpen && HasAuthority())
    {
        bOpen = true;
        OnRep_OpenState();
    }
}

void APEDoorActor::Close()
{
    if (bOpen && HasAuthority())
    {
        bOpen = false;
        OnRep_OpenState();
    }
}
void APEDoorActor::ToggleOpen()
{
    if (HasAuthority())
    {
        bOpen = !bOpen;
        OnRep_OpenState();   // 서버에서도 즉시 재생
    }
    // 클라는 RepNotify로 재생
}

void APEDoorActor::OnRep_OpenState()
{
    BeginMove(bOpen);
    if (bOpen && bAutoClose && HasAuthority())
    {
        ScheduleAutoClose();
    }
    else if (!bOpen)
    {
        ClearAutoClose();
    }
}

void APEDoorActor::BeginMove(bool bToOpen)
{
    bMoving = true;
    MoveStartTime = GetWorld()->GetTimeSeconds();
    StartLoc = DoorMesh->GetRelativeLocation();

    const FVector Offset = DirVectorLocal() * Distance;
    TargetLoc = bToOpen ? (BaseLoc + Offset) : BaseLoc;
}

void APEDoorActor::ApplyAtAlpha(float Alpha)
{
    // 선형 보간(필요하면 이징 커브로 교체 가능)
    const FVector NewLoc = FMath::Lerp(StartLoc, TargetLoc, Alpha);
    DoorMesh->SetRelativeLocation(NewLoc);
}

void APEDoorActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bMoving) return;

    const double Elapsed = GetWorld()->GetTimeSeconds() - MoveStartTime;
    const float  Alpha = FMath::Clamp(static_cast<float>(Elapsed / FMath::Max(0.05, (double)MoveTime)), 0.f, 1.f);

    ApplyAtAlpha(Alpha);

    if (Alpha >= 1.f)
    {
        bMoving = false;
    }
}

void APEDoorActor::ClearAutoClose()
{
    if (AutoCloseTimer.IsValid())
    {
        GetWorldTimerManager().ClearTimer(AutoCloseTimer);
    }
}

void APEDoorActor::ScheduleAutoClose()
{
    ClearAutoClose();
    GetWorldTimerManager().SetTimer(
        AutoCloseTimer,
        [this]()
        {
            if (IsValid(this) && HasAuthority() && bOpen)
            {
                ToggleOpen(); // 닫기
            }
        },
        AutoCloseDelay,
        false
    );
}