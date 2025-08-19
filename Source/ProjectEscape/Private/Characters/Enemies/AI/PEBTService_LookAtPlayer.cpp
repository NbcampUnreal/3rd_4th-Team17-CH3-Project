

#include "Characters/Enemies/AI/PEBTService_LookAtPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

UPEBTService_LookAtPlayer::UPEBTService_LookAtPlayer()
{
    NodeName = TEXT("Look At Player");
    bNotifyTick = true;  // �� ƽ���� TickNode ȣ��
    Interval = 0.0f;     // �� �����Ӹ��� ���� (0.0f�� ����)
    RandomDeviation = 0.0f;
}

void UPEBTService_LookAtPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // AI ��Ʈ�ѷ� ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return;
    }

    // AI �� ��������
    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
    {
        return;
    }

    // �����忡�� Ÿ�� ����(�÷��̾�) ��������
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

    // Ÿ�� ���� ���
    FVector AILocation = AIPawn->GetActorLocation();
    FVector TargetLocation = TargetActor->GetActorLocation();
    FVector Direction = (TargetLocation - AILocation).GetSafeNormal();

    // Ÿ�� ȸ���� ��� (Yaw�� ����Ͽ� ���� ȸ����)
    FRotator TargetRotation = Direction.Rotation();
    TargetRotation.Pitch = 0.0f; // ���� ȸ�� ����
    TargetRotation.Roll = 0.0f;  // �� ȸ�� ����

    // ���� ȸ�������� Ÿ�� ȸ�������� �ε巴�� ����
    FRotator CurrentRotation = AIPawn->GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);

    // ���ο� ȸ���� ����
    AIPawn->SetActorRotation(NewRotation);
}