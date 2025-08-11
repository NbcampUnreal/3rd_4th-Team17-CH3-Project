// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "PEAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class PROJECTESCAPE_API APEAIController : public AAIController
{
	GENERATED_BODY()

public:
	APEAIController();

	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const // 블랙보드 컴포넌트를 반환하는 강제 인라인 함수
	{
		return BlackboardComp;
	}

	void StartBehaviorTree();
	void EndInvestigating();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float SightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float LoseSightRadius = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float SightAngle = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float SightDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float InvestigateDuration = 5.0f; // 플레이어를 놓쳤을 때 조사하는 시간

	FTimerHandle InvestigateTimerHandle; // 조사 타이머 핸들

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	UAIPerceptionComponent* AIPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Blackboard")
	UBlackboardComponent* BlackboardComp;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

};
