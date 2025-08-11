// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEBTTask_AttackPlayer.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UPEBTTask_AttackPlayer::UPEBTTask_AttackPlayer()
{
    NodeName = TEXT("Attack Player");
    bNotifyTick = true; // Tick 이벤트 사용 (공격 지속 시간 관리)
}

EBTNodeResult::Type UPEBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* MyPawn = AIController->GetPawn();
    if (!MyPawn)
    {
        return EBTNodeResult::Failed;
    }

    // 블랙보드에서 타겟 액터(플레이어) 가져오기
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
    if (!TargetActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack Failed: No Target Actor"));
        return EBTNodeResult::Failed;
    }

    // 플레이어와의 거리 체크
    float DistanceToPlayer = FVector::Dist(MyPawn->GetActorLocation(), TargetActor->GetActorLocation());
    if (DistanceToPlayer > AttackRange)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack Failed: Player too far (%.1f > %.1f)"), DistanceToPlayer, AttackRange);
        return EBTNodeResult::Failed;
    }

    // 플레이어를 바라보도록 회전
    FVector Direction = (TargetActor->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal();
    FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    MyPawn->SetActorRotation(FMath::RInterpTo(MyPawn->GetActorRotation(), LookRotation, GetWorld()->GetDeltaSeconds(), 5.0f));

    // 공격 실행 (로그)
    UE_LOG(LogTemp, Warning, TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer);

    // 화면에도 표시
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
            FString::Printf(TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer));
    }

    // 공격 완료까지 대기 (AttackDuration 시간 후 Success 반환) <- 애니메이션 재생 시간에 따라 조정
    // 간단한 구현을 위해 바로 성공 반환
    return EBTNodeResult::Succeeded;
}