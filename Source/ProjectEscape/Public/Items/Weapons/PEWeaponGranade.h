// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEWeaponProjectile.h"
#include "PEWeaponGranade.generated.h"

UCLASS()
class PROJECTESCAPE_API APEWeaponGranade : public APEWeaponProjectile
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEWeaponGranade();

protected:
	virtual void BeginPlay() override;

	/* Weapon 관련 섹션 */
public:
	virtual void DoPrimaryAction(AActor* Holder) override;
	virtual void CompletePrimaryAction(AActor* Holder) override;
	virtual void DoTertiaryAction(AActor* Holder) override;

	/* Granade 관련 섹션 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Granade")
	float ThrowingReadyTime = 0.3f; // 그레네이드 던지기 준비 시간

	FTimerHandle ThrowingReadyTimerHandle;
	
	bool bIsThrowingReady = false;
	bool bIsReleasedButton = false;

	// 투척 준비 완료 함수
	UFUNCTION()
	void ThrowingReady(AActor* Holder);

	UFUNCTION()
	void PerformThrowing(AActor* Holder);
};
