// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEBTTask_ChaseMoveTo.h"
#include "Characters/Enemies/AI/PEAICharacter.h"
#include "Characters/Enemies/AI/PEAIBossCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

UPEBTTask_ChaseMoveTo::UPEBTTask_ChaseMoveTo()
{
    NodeName = TEXT("Chase Move To");
    bNotifyTick = false;              
    bNotifyTaskFinished = true; // TaskFinished 알림 활성화
}

EBTNodeResult::Type UPEBTTask_ChaseMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APEAICharacter* AICharacter = Cast<APEAICharacter>(AIController->GetPawn());
    if (!AICharacter) return EBTNodeResult::Failed;

    if (APEAIBossCharacter* BossCharacter = Cast<APEAIBossCharacter>(AICharacter))
    {
        if (BossCharacter->bIsInPhaseTransition)
        {
            UE_LOG(LogTemp, Warning, TEXT("Boss is in phase transition, canceling chase"));
            return EBTNodeResult::Failed; // 또는 InProgress로 대기
        }
    }

    UCharacterMovementComponent* MovementComp = AICharacter->GetCharacterMovement();
    if (!MovementComp) return EBTNodeResult::Failed;

    // 1. 캐릭터 BP의 AttackRange 값으로 AcceptableRadius 설정
    float CharacterAttackRange = AICharacter->AttackRange;
    if (CharacterAttackRange > 0.0f)
    {
        AcceptableRadius = FMath::Max(CharacterAttackRange - OffsetDistance, 50.0f);

        UE_LOG(LogTemp, Log, TEXT("%s: Chase AcceptableRadius set to %.1f"), *AICharacter->GetName(), CharacterAttackRange);
    }

	AICharacter->SetMovementSpeed(AICharacter->RunSpeed);

    return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UPEBTTask_ChaseMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    // Chase 완료 시 기본 속도로 복원 (300)
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController)
    {
        APEAICharacter* AICharacter = Cast<APEAICharacter>(AIController->GetPawn());
        if (AICharacter)
        {
			AICharacter->SetMovementSpeed(AICharacter->WalkSpeed); // 기본 속도로 복원
        }
    }

    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}