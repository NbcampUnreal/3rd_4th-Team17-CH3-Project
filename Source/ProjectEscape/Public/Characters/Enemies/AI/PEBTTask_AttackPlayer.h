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
    float AttackRange = 150.0f; // ���� ������ �ִ� �Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
    float AttackCoolTime = 2.0f; // ���� ��Ÿ��

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Attack")
    float AttackDuration = 0.0f; // ���� �ִϸ��̼�/������ �ð�(���� �ִϸ��̼� �߰� �� Ȱ���� ����)
};
