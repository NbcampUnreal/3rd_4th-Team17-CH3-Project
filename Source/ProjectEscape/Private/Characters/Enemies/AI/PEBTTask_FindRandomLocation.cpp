// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEBTTask_FindRandomLocation.h"
#include "Characters/Enemies/AI/PEAIController.h"
#include "Characters/Enemies/AI/PEAIBossCharacter.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UPEBTTask_FindRandomLocation::UPEBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UPEBTTask_FindRandomLocation, LocationKey));
}

EBTNodeResult::Type UPEBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 1�ܰ�: �ʿ��� �͵� ��������
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(!AIController)
	{
		return EBTNodeResult::Failed;
	}

	APawn* MyPawn = AIController->GetPawn();
	if(!MyPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (APEAIBossCharacter* BossCharacter = Cast<APEAIBossCharacter>(MyPawn))
	{
		if (BossCharacter->bIsInPhaseTransition)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss is in phase transition, skipping random location search"));
			return EBTNodeResult::Failed; // �Ǵ� InProgress�� ���
		}
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if(!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	// 2�ܰ�: ���� ��ġ ã��
	FNavLocation RandomLocation;
	bool bFound = NavSystem->GetRandomPointInNavigableRadius(MyPawn->GetActorLocation(), SearchRadius, RandomLocation);

	// 3�ܰ�: ã������ �����忡 ��ġ ����
	if (bFound)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if(BlackboardComp)
		{
			BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
			return EBTNodeResult::Succeeded; // ���������� ��ġ�� ã����
		}
	}
	return EBTNodeResult::Failed;
}