// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Components/PEAttackBaseComponent.h"

#include "Core/PELogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

UPEAttackBaseComponent::UPEAttackBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPEAttackBaseComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UPEAttackBaseComponent::ExcuteAttack(const FPEAttackStats& AttackStats)
{
	if (!AttackStartPoint)
	{
		UE_LOG(LogPE, Error, TEXT("PEAttackComponentBase::ExcuteAttack: AttackStartPoint is not set!"));
		return false;
	}

	const FVector& StartLocation = AttackStartPoint->GetComponentLocation();
	const FRotator& StartRotation = AttackStartPoint->GetComponentRotation();
	FVector Direction = StartRotation.Vector();

	Direction = ApplyAccuracyDeviation(Direction, AttackStats.SpreadAngle);

	PerformAttack(AttackStats, StartLocation, Direction);
	return true;
}

bool UPEAttackBaseComponent::ExcuteAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction)
{
	FVector FinalDirection = ApplyAccuracyDeviation(Direction, AttackStats.SpreadAngle);

	PerformAttack(AttackStats, StartLocation, FinalDirection);
	return true;
}

void UPEAttackBaseComponent::PlayParticleEffect(UParticleSystem* ParticleEffect, const FVector& Location,
	const FRotator& Rotation)
{
	if (!ParticleEffect)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			World,
			ParticleEffect,
			Location,
			Rotation,
			true  // AutoDestroy
		);
	}
}

void UPEAttackBaseComponent::PlayParticleEffect(UParticleSystem* ParticleEffect)
{
	PlayParticleEffect(ParticleEffect, AttackStartPoint->GetComponentLocation(), AttackStartPoint->GetComponentRotation());	
}

void UPEAttackBaseComponent::PlaySoundEffect(USoundBase* SoundEffect, const FVector& Location)
{
	if (!SoundEffect)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(
			World,
			SoundEffect,
			Location,
			1.0f,  // VolumeMultiplier
			1.0f   // PitchMultiplier
		);
	}
}

void UPEAttackBaseComponent::PlaySoundEffect(USoundBase* SoundEffect)
{
	PlaySoundEffect(SoundEffect, AttackStartPoint->GetComponentLocation());
}

void UPEAttackBaseComponent::SetAttackStartPoint(USceneComponent* NewStartPoint)
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

void UPEAttackBaseComponent::PerformAttack(const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction)
{
	// NOTE: 실제 구현은 자식 클래스에 작성해야 합니다.
	UE_LOG(LogPE, Warning, TEXT("PEAttackComponentBase::PerformAttack: This function should be overridden in derived classes."));
}

FVector UPEAttackBaseComponent::ApplyAccuracyDeviation(const FVector& OriginalDirection, const float& SpreadAngle) const
{
	if (SpreadAngle <= 0.0f)
	{
		return OriginalDirection;
	}

	const float RandomPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);
	const float RandomYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
	
	const FRotator DirectionRotator = OriginalDirection.Rotation();
	const FRotator RandomOffset(RandomPitch, RandomYaw, 0.0f);
	const FRotator FinalRotation = DirectionRotator + RandomOffset;
	
	return FinalRotation.Vector();
}
