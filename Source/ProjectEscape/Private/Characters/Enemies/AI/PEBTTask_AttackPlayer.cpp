// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEBTTask_AttackPlayer.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "Characters/Enemies/AI/PEAICharacter.h"
#include "Characters/Enemies/AI/PEAIBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

UPEBTTask_AttackPlayer::UPEBTTask_AttackPlayer()
{
    NodeName = TEXT("Attack Player");
    bCreateNodeInstance = true;
}


EBTNodeResult::Type UPEBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // OwnerComp 캐싱 (AI 사망 시 FinishLatentTask 호출용)
    CachedOwnerComp = &OwnerComp;

    // 기존 타이머 정리
    ClearAttackTimer();

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

    // AI 사망 시 델리게이트 바인딩
    if (APEAICharacter* AICharacter = Cast<APEAICharacter>(MyPawn))
    {
        // 기존 바인딩 해제 후 새로 바인딩
        if (PawnDeathDelegateHandle.IsValid())
        {
            AICharacter->OnPawnDeath.Remove(PawnDeathDelegateHandle);
        }

        PawnDeathDelegateHandle = AICharacter->OnPawnDeath.AddUObject(
            this, &UPEBTTask_AttackPlayer::ClearAttackTimer);
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

    APEAICharacter* AICharacter = Cast<APEAICharacter>(MyPawn);
	float CharacterAttackRange = AICharacter ? AICharacter->AttackRange : 1500.0f; // 기본 공격 범위 설정

    // 플레이어와의 거리 체크
    float DistanceToPlayer = FVector::Dist(MyPawn->GetActorLocation(), TargetActor->GetActorLocation());
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float LastAttackTime = BlackboardComp->GetValueAsFloat(TEXT("LastAttackTime"));
    FVector AttackStartLocation = AICharacter->AttackStart->GetComponentLocation();
    //FVector Direction = (TargetActor->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal();
    FVector Direction = (TargetActor->GetActorLocation() - AttackStartLocation).GetSafeNormal();
    FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

    MyPawn->SetActorRotation(LookRotation);
	AttackCoolTime = AICharacter ? AICharacter->AttackCoolTime : 2.0f; 
    // 쿨다운 체크
    if (CurrentTime - LastAttackTime < AttackCoolTime)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Attack CoolTime: %.1f초"),
            AttackCoolTime - (CurrentTime - LastAttackTime));

        float RemainingTime = AttackCoolTime - (CurrentTime - LastAttackTime);

        TWeakObjectPtr<UPEBTTask_AttackPlayer> WeakThis(this);
        GetWorld()->GetTimerManager().SetTimer(
            AttackTimerHandle,
            [WeakThis, &OwnerComp]()
            {
                if (WeakThis.IsValid())
                {       
                    WeakThis->FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                }
            },
            RemainingTime,
            false
        );

        return EBTNodeResult::InProgress;
    }

    // 공격 실행 (로그)
    UE_LOG(LogTemp, Warning, TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer);
    if (AICharacter)
    {
        MyPawn->SetActorRotation(LookRotation);
        if (APEAIBossCharacter* BossCharacter = Cast<APEAIBossCharacter>(AICharacter))
        {
            if (BossCharacter->bIsInPhaseTransition || BossCharacter->bIsUsingSpecialSkill)
            {
                UE_LOG(LogTemp, Warning, TEXT("Boss is in phase transition, skipping attack"));
                return EBTNodeResult::Failed; // 또는 InProgress로 대기
            }
            BossCharacter->PerformBossAttack(DistanceToPlayer);
        }
        else
        {
            AICharacter->PerformAttack();
            AICharacter->AttackComponent->SetAttackStartPoint(AICharacter->AttackStart); // 공격 시작 위치 설정
            AICharacter->AttackComponent->PlayParticleEffect(AICharacter->AttackParticleEffect, AICharacter->AttackStart->GetComponentLocation(), AICharacter->AttackStart->GetComponentRotation()); // 공격 이펙트 재생
            AICharacter->AttackComponent->PlaySoundEffect(AICharacter->AttackSoundEffect, AICharacter->AttackStart->GetComponentLocation()); // 공격 사운드 재생
        }
        //MyPawn->GetActorLocation(
        FPEAttackStats AttackStats;
        AttackStats.AttackRange = AICharacter->AttackRange; // 공격 범위 설정
        AttackStats.DamageAmount = AICharacter->AttackAmount; // 임시 데미지 값 설정
        AICharacter->AttackComponent->ExcuteAttack(AttackStats, AICharacter->AttackStart->GetComponentLocation(), Direction); // 공격 실행
	}

    // 화면에도 표시
    if (GEngine)
    {
#if UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
            FString::Printf(TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer));
#endif
    }

    BlackboardComp->SetValueAsFloat(TEXT("LastAttackTime"), CurrentTime); //마지막 공격 시간 업데이트

    // 공격 완료까지 대기 (AttackDuration 시간 후 Success 반환) <- 애니메이션 재생 시간에 따라 조정
    // 간단한 구현을 위해 바로 성공 반환

    return EBTNodeResult::Succeeded;
}

void UPEBTTask_AttackPlayer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    // 정상 완료 시 타이머 정리
    ClearAttackTimer();

    // 캐시 정리
    CachedOwnerComp.Reset();

    // 델리게이트 바인딩 해제
    if (PawnDeathDelegateHandle.IsValid())
    {
        if (AAIController* AIController = OwnerComp.GetAIOwner())
        {
            if (APEAICharacter* AICharacter = Cast<APEAICharacter>(AIController->GetPawn()))
            {
                AICharacter->OnPawnDeath.Remove(PawnDeathDelegateHandle);
            }
        }
        PawnDeathDelegateHandle.Reset();
    }

    Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}


void UPEBTTask_AttackPlayer::OnInstanceDestroyed(UBehaviorTreeComponent& OwnerComp)
{
    // 비정상 종료 시 타이머 정리 (안전장치)
    ClearAttackTimer();

    // 캐시 정리
    CachedOwnerComp.Reset();

    // 델리게이트 바인딩 해제
    if (PawnDeathDelegateHandle.IsValid())
    {
        if (AAIController* AIController = OwnerComp.GetAIOwner())
        {
            if (APEAICharacter* AICharacter = Cast<APEAICharacter>(AIController->GetPawn()))
            {
                AICharacter->OnPawnDeath.Remove(PawnDeathDelegateHandle);
            }
        }
        PawnDeathDelegateHandle.Reset();
    }

    Super::OnInstanceDestroyed(OwnerComp);
}

EBTNodeResult::Type UPEBTTask_AttackPlayer::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 타이머 정리
    ClearAttackTimer();
    // 캐시 정리
    CachedOwnerComp.Reset();
    // 델리게이트 바인딩 해제
    if (PawnDeathDelegateHandle.IsValid())
    {
        if (AAIController* AIController = OwnerComp.GetAIOwner())
        {
            if (APEAICharacter* AICharacter = Cast<APEAICharacter>(AIController->GetPawn()))
            {
                AICharacter->OnPawnDeath.Remove(PawnDeathDelegateHandle);
            }
        }
        PawnDeathDelegateHandle.Reset();
    }
    return EBTNodeResult::Aborted;
}

void UPEBTTask_AttackPlayer::ClearAttackTimer()
{
    // 타이머 정리
    if (AttackTimerHandle.IsValid())
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(AttackTimerHandle);
            UE_LOG(LogTemp, Log, TEXT("Attack Timer Cleared"));
        }
    }

    // InProgress 상태였다면 강제 종료 (AI 사망 시 대응)
    if (CachedOwnerComp.IsValid() && IsValid(CachedOwnerComp.Get()))
    {
        FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Aborted);
    }
}
