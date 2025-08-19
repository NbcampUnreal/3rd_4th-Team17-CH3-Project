// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PEBTService_LookAtPlayer.generated.h"


UCLASS()
class PROJECTESCAPE_API UPEBTService_LookAtPlayer : public UBTService
{
	GENERATED_BODY()
	
public:
    UPEBTService_LookAtPlayer();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // ȸ�� �ӵ� (���� Ŭ���� ������ ȸ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look At Player")
    float RotationSpeed = 5.0f;

    // ������ Ű �̸� (�÷��̾� Ÿ��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look At Player")
    FName TargetKeyName = TEXT("TargetActor");
};
