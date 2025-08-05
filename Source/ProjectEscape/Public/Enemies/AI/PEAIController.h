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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float SightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float LoseSightRadius = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float SightAngle = 90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight")
	float SightDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolCycle = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolRepeatTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Chase")
	float ChaseDistance = 100.0f; // 추적 완료처리되는 플레이어와의 거리

protected:
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
	void MoveToRandomLocation();

	FTimerHandle RandomMoveTimer;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MoveRadius = 1000.0f;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	bool bIsChasing = false;
	FTimerHandle ChaseTimer;

	void StartChasing(AActor* Target);
	void StopChasing();
	void UpdateChase();
};
