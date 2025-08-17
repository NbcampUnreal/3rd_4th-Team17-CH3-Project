// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "PEBTTask_ChaseMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTESCAPE_API UPEBTTask_ChaseMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
    UPEBTTask_ChaseMoveTo();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

    // AttackRange에서 뺄 여유 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase Settings")
    float OffsetDistance = 50.0f;

private:
};
