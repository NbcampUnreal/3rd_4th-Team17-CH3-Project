// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEAttackComponentBase.h"

#include "Core/PELogChannels.h"

UPEAttackComponentBase::UPEAttackComponentBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPEAttackComponentBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UPEAttackComponentBase::ExcuteAttack(const FPEAttackStats& AttackStats)
{
	if (!AttackStartPoint)
	{
		UE_LOG(LogPE, Error, TEXT("PEAttackComponentBase::ExcuteAttack: AttackStartPoint is not set!"));
		return false;
	}

	const FVector& StartLocation = AttackStartPoint->GetComponentLocation();
	const FRotator& StartRotation = AttackStartPoint->GetComponentRotation();
	const FVector& Direction = StartRotation.Vector();

	PerformAttack(AttackStats, StartLocation, Direction);
	return true;
}

bool UPEAttackComponentBase::ExcuteAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction)
{
	PerformAttack(AttackStats, StartLocation, Direction);
	return true;
}

void UPEAttackComponentBase::SetAttackStartPoint(USceneComponent* NewStartPoint)
{
	if (NewStartPoint)
	{
		AttackStartPoint = NewStartPoint;
	}
	else
	{
		UE_LOG(LogPE, Warning, TEXT("PEAttackComponentBase::SetAttackStartPoint: NewStartPoint is null!"));
	}
}

void UPEAttackComponentBase::PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction)
{
	// NOTE: 실제 구현은 자식 클래스에 작성해야 합니다.
	UE_LOG(LogPE, Warning, TEXT("PEAttackComponentBase::PerformAttack: This function should be overridden in derived classes."));
}


