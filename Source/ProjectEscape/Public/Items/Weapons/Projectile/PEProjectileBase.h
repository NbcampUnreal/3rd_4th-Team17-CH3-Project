// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Combat/Components/PEAttackBaseComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PEProjectileBase.generated.h"

struct FPEAttackStats;

UCLASS()
class PROJECTESCAPE_API APEProjectileBase : public AActor
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEProjectileBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Components 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UBoxComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/* Projectile 관련 섹션 */
public:
	virtual void Launch(AActor* InInstigator, const FPEAttackStats& AttackStats, const FVector& StartLocation, const FVector& Direction);
	AActor* GetInstigator() const;
protected:
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	FPEAttackStats ProjectileStats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileLifetime;

	UPROPERTY()
	FTimerHandle ProjectileLifetimeTimer;

	UPROPERTY()
	TObjectPtr<AActor> InstigatorActor;

protected:
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnProjectileExpired();

	/* Effect 관련 섹션 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* HitSound;
};
