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
    // OwnerComp ĳ�� (AI ��� �� FinishLatentTask ȣ���)
    CachedOwnerComp = &OwnerComp;

    // ���� Ÿ�̸� ����
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

    // AI ��� �� ��������Ʈ ���ε�
    if (APEAICharacter* AICharacter = Cast<APEAICharacter>(MyPawn))
    {
        // ���� ���ε� ���� �� ���� ���ε�
        if (PawnDeathDelegateHandle.IsValid())
        {
            AICharacter->OnPawnDeath.Remove(PawnDeathDelegateHandle);
        }

        PawnDeathDelegateHandle = AICharacter->OnPawnDeath.AddUObject(
            this, &UPEBTTask_AttackPlayer::ClearAttackTimer);
    }

    // �����忡�� Ÿ�� ����(�÷��̾�) ��������
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
	float CharacterAttackRange = AICharacter ? AICharacter->AttackRange : 1500.0f; // �⺻ ���� ���� ����

    // �÷��̾���� �Ÿ� üũ
    float DistanceToPlayer = FVector::Dist(MyPawn->GetActorLocation(), TargetActor->GetActorLocation());
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float LastAttackTime = BlackboardComp->GetValueAsFloat(TEXT("LastAttackTime"));
    FVector AttackStartLocation = AICharacter->AttackStart->GetComponentLocation();
    //FVector Direction = (TargetActor->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal();
    FVector Direction = (TargetActor->GetActorLocation() - AttackStartLocation).GetSafeNormal();
    FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

    MyPawn->SetActorRotation(LookRotation);
	AttackCoolTime = AICharacter ? AICharacter->AttackCoolTime : 2.0f; 
    // ��ٿ� üũ
    if (CurrentTime - LastAttackTime < AttackCoolTime)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI Attack CoolTime: %.1f��"),
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

    // ���� ���� (�α�)
    UE_LOG(LogTemp, Warning, TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer);
    if (AICharacter)
    {
        MyPawn->SetActorRotation(LookRotation);
        if (APEAIBossCharacter* BossCharacter = Cast<APEAIBossCharacter>(AICharacter))
        {
            if (BossCharacter->bIsInPhaseTransition || BossCharacter->bIsUsingSpecialSkill)
            {
                UE_LOG(LogTemp, Warning, TEXT("Boss is in phase transition, skipping attack"));
                return EBTNodeResult::Failed; // �Ǵ� InProgress�� ���
            }
            BossCharacter->PerformBossAttack(DistanceToPlayer);
        }
        else
        {
            AICharacter->PerformAttack();
            AICharacter->AttackComponent->SetAttackStartPoint(AICharacter->AttackStart); // ���� ���� ��ġ ����
            AICharacter->AttackComponent->PlayParticleEffect(AICharacter->AttackParticleEffect, AICharacter->AttackStart->GetComponentLocation(), AICharacter->AttackStart->GetComponentRotation()); // ���� ����Ʈ ���
            AICharacter->AttackComponent->PlaySoundEffect(AICharacter->AttackSoundEffect, AICharacter->AttackStart->GetComponentLocation()); // ���� ���� ���
        }
        //MyPawn->GetActorLocation(
        FPEAttackStats AttackStats;
        AttackStats.AttackRange = AICharacter->AttackRange; // ���� ���� ����
        AttackStats.DamageAmount = AICharacter->AttackAmount; // �ӽ� ������ �� ����
        AICharacter->AttackComponent->ExcuteAttack(AttackStats, AICharacter->AttackStart->GetComponentLocation(), Direction); // ���� ����
	}

    // ȭ�鿡�� ǥ��
    if (GEngine)
    {
#if UE_BUILD_DEBUG
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
            FString::Printf(TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer));
#endif
    }

    BlackboardComp->SetValueAsFloat(TEXT("LastAttackTime"), CurrentTime); //������ ���� �ð� ������Ʈ

    // ���� �Ϸ���� ��� (AttackDuration �ð� �� Success ��ȯ) <- �ִϸ��̼� ��� �ð��� ���� ����
    // ������ ������ ���� �ٷ� ���� ��ȯ

    return EBTNodeResult::Succeeded;
}

void UPEBTTask_AttackPlayer::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
    // ���� �Ϸ� �� Ÿ�̸� ����
    ClearAttackTimer();

    // ĳ�� ����
    CachedOwnerComp.Reset();

    // ��������Ʈ ���ε� ����
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
    // ������ ���� �� Ÿ�̸� ���� (������ġ)
    ClearAttackTimer();

    // ĳ�� ����
    CachedOwnerComp.Reset();

    // ��������Ʈ ���ε� ����
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
    // Ÿ�̸� ����
    ClearAttackTimer();
    // ĳ�� ����
    CachedOwnerComp.Reset();
    // ��������Ʈ ���ε� ����
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
    // Ÿ�̸� ����
    if (AttackTimerHandle.IsValid())
    {
        if (UWorld* World = GetWorld())
        {
            World->GetTimerManager().ClearTimer(AttackTimerHandle);
            UE_LOG(LogTemp, Log, TEXT("Attack Timer Cleared"));
        }
    }

    // InProgress ���¿��ٸ� ���� ���� (AI ��� �� ����)
    if (CachedOwnerComp.IsValid() && IsValid(CachedOwnerComp.Get()))
    {
        FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Aborted);
    }
}
