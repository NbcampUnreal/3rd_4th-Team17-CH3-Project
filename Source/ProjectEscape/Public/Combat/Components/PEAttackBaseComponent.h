// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Interface/PEAttackable.h"
#include "Components/ActorComponent.h"
#include "PEAttackBaseComponent.generated.h"

USTRUCT()
struct PROJECTESCAPE_API FPEAttackStats
{
	GENERATED_BODY()

	float DamageAmount = 10.0f; 
	float AttackRange = 1000.0f; 
	ECollisionChannel CollisionChannel = ECC_Pawn;
};

/*
 *	공격을 실행할 수 있는 액터에 부착하여 사용하는 컴포넌트 클래스입니다.
 *	공격을 위한 Ray나 Collision을 사용하여 공격을 실행하는 기능을 제공합니다.
 *	공격 쿨타임같은 기타 로직은 각 액터에서 구현해야 합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEAttackBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEAttackBaseComponent();

protected:
	virtual void BeginPlay() override;

	/* Attack 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Attack")
	TObjectPtr<USceneComponent> AttackStartPoint;
	
public:
	virtual bool ExcuteAttack(const FPEAttackStats& AttackStats);	//AttackStartPoint 기준으로 공격을 실행합니다. (카메라같은 기준 액터를 사용할 때)
	virtual bool ExcuteAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction);

	void SetAttackStartPoint(USceneComponent* NewStartPoint);

protected:
	virtual void PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction);
};
