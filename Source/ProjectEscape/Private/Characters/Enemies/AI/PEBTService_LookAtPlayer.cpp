

#include "Characters/Enemies/AI/PEBTService_LookAtPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

UPEBTService_LookAtPlayer::UPEBTService_LookAtPlayer()
{
    NodeName = TEXT("Look At Player");
    bNotifyTick = true;  // 매 틱마다 TickNode 호출
    Interval = 0.0f;     // 매 프레임마다 실행 (0.0f로 설정)
    RandomDeviation = 0.0f;
}

void UPEBTService_LookAtPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // AI 컨트롤러 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return;
    }

    // AI 폰 가져오기
    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
    {
        return;
    }

    // 블랙보드에서 타겟 액터(플레이어) 가져오기
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKeyName));
    if (!TargetActor)
    {
        return;
    }

    // 타겟 방향 계산
    FVector AILocation = AIPawn->GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();
    FVector Direction = (TargetLocation - AILocation).GetSafeNormal();

    // 타겟 회전값 계산 (Yaw만 사용하여 수평 회전만)
    FRotator TargetRotation = Direction.Rotation();
    TargetRotation.Pitch = 0.0f; // 수직 회전 제거
    TargetRotation.Roll = 0.0f;  // 롤 회전 제거

    // 현재 회전값에서 타겟 회전값으로 부드럽게 보간
    FRotator CurrentRotation = AIPawn->GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);

    // 새로운 회전값 적용
    AIPawn->SetActorRotation(NewRotation);
}