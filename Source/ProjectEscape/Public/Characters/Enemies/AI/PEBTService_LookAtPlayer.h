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

    // 회전 속도 (값이 클수록 빠르게 회전)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look At Player")
    float RotationSpeed = 5.0f;

    // 블랙보드 키 이름 (플레이어 타겟)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Look At Player")
    FName TargetKeyName = TEXT("TargetActor");
};
