// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Interface/PEAttackable.h"
#include "Components/ActorComponent.h"
#include "Core/PEPhysics.h"
#include "PEAttackBaseComponent.generated.h"

class APEProjectileBase;

USTRUCT()
struct PROJECTESCAPE_API FPEAttackStats
{
	GENERATED_BODY()

	TSubclassOf<APEProjectileBase> ProjectileClass; 
	float DamageAmount = 10.0f; 
	float AttackRange = 1000.0f;
	float SpreadAngle = 0.0f;
	float ProjectileSpeed = 1000.0f;
	float ExplosionRadius = 200.0f;
	float ExplosionDelay = 3.0f;
	ECollisionChannel HitscanChannel = CCHANNEL_ENEMYHITSCAN;
	ECollisionChannel ProjectileCollisionChannel = CCHANNEL_ENEMYPROJECTILE;
};

/*
 *	������ ������ �� �ִ� ���Ϳ� �����Ͽ� ����ϴ� ������Ʈ Ŭ�����Դϴ�.
 *	������ ���� Ray�� Collision�� ����Ͽ� ������ �����ϴ� ����� �����մϴ�.
 *	���� ��Ÿ�Ӱ��� ��Ÿ ������ �� ���Ϳ��� �����ؾ� �մϴ�.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEAttackBaseComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle ���� ���� */
public:
	UPEAttackBaseComponent();

protected:
	virtual void BeginPlay() override;

	/* Attack ���� ���� */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Attack")
	TObjectPtr<USceneComponent> AttackStartPoint;
	
public:
	virtual bool ExcuteAttack(const FPEAttackStats& AttackStats);	//AttackStartPoint �������� ������ �����մϴ�. (ī�޶��� ���� ���͸� ����� ��)
	virtual bool ExcuteAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction);

	void PlayParticleEffect(UParticleSystem* ParticleEffect, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator);
	void PlayParticleEffect(UParticleSystem* ParticleEffect);
	void PlaySoundEffect(USoundBase* SoundEffect, const FVector& Location);
	void PlaySoundEffect(USoundBase* SoundEffect);
	void SetAttackStartPoint(USceneComponent* NewStartPoint);
	

protected:
	FVector ApplyAccuracyDeviation(const FVector& OriginalDirection, const float& SpreadAngle) const;
	
	virtual void PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction);
};
