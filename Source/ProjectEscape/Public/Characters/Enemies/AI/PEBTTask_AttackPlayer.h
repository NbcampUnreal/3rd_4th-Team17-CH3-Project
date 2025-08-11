// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PEBTTask_AttackPlayer.generated.h"

UCLASS()
class PROJECTESCAPE_API UPEBTTask_AttackPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UPEBTTask_AttackPlayer();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
    float AttackRange = 150.0f; // 공격 가능한 최대 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
    float AttackCoolTime = 2.0f; // 공격 쿨타임

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
    float AttackDuration = 0.0f; // 공격 애니메이션/딜레이 시간(추후 애니메이션 추가 시 활용할 예정)
};
