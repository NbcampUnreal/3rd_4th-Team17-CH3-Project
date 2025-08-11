// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEBTTask_AttackPlayer.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

UPEBTTask_AttackPlayer::UPEBTTask_AttackPlayer()
{
    NodeName = TEXT("Attack Player");
    // bNotifyTick = true; // Tick �̺�Ʈ ��� (���� ���� �ð� ����)
}


EBTNodeResult::Type UPEBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (AttackTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
    }

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

    float CurrentTime = GetWorld()->GetTimeSeconds();
    float LastAttackTime = BlackboardComp->GetValueAsFloat(TEXT("LastAttackTime"));

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


    // �÷��̾���� �Ÿ� üũ
    float DistanceToPlayer = FVector::Dist(MyPawn->GetActorLocation(), TargetActor->GetActorLocation());
    if (DistanceToPlayer > AttackRange)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack Fail: Player too far (%.1f > %.1f)"), DistanceToPlayer, AttackRange);
        return EBTNodeResult::Failed;
    }

    // �÷��̾ �ٶ󺸵��� ȸ��
    FVector Direction = (TargetActor->GetActorLocation() - MyPawn->GetActorLocation()).GetSafeNormal();
    FRotator LookRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    MyPawn->SetActorRotation(FMath::RInterpTo(MyPawn->GetActorRotation(), LookRotation, GetWorld()->GetDeltaSeconds(), 5.0f));

    // ���� ���� (�α�)
    UE_LOG(LogTemp, Warning, TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer);

    // ȭ�鿡�� ǥ��
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
            FString::Printf(TEXT("AI ATTACKING PLAYER! Distance: %.1f"), DistanceToPlayer));
    }

    BlackboardComp->SetValueAsFloat(TEXT("LastAttackTime"), CurrentTime); //������ ���� �ð� ������Ʈ

    // ���� �Ϸ���� ��� (AttackDuration �ð� �� Success ��ȯ) <- �ִϸ��̼� ��� �ð��� ���� ����
    // ������ ������ ���� �ٷ� ���� ��ȯ

    return EBTNodeResult::Succeeded;
}