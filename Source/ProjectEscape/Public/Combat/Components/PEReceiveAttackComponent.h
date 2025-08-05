// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "PEReceiveAttackComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEReceiveAttackComponent : public UBoxComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEReceiveAttackComponent();

protected:
	virtual void BeginPlay() override;

	/* Receive Attack 관련 섹션 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Receive Attack")
	TObjectPtr<AActor> DamageTarget; // 공격을 받는 액터
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Receive Attack")
	float DamageMultiplier;
	
	void ReceiveDamage(float DamageAmount, const FVector& HitLocation, const FVector& HitNormal, AActor* Instigator);
};
