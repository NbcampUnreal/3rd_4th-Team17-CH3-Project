// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PEBTTask_FindRandomLocation.generated.h"


UCLASS()
class PROJECTESCAPE_API UPEBTTask_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPEBTTask_FindRandomLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	struct FBlackboardKeySelector LocationKey; // 블랙보드에 저장할 위치 키

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Search")
	float SearchRadius = 1000.0f; // 랜덤 위치를 찾을 반경
	
};
