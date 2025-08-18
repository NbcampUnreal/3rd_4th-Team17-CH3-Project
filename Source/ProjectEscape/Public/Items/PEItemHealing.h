// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PEItemUseable.h"
#include "PEItemHealing.generated.h"

UCLASS()
class PROJECTESCAPE_API APEItemHealing : public APEItemUseable
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	APEItemHealing();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	/* IPEQuickSlotItem 인터페이스 선언 */
public:
	virtual void OnDropped(const FVector& Location, const FRotator& Rotation) override;
	
	/* IPEUseable 인터페이스 선언 */
public:
	virtual void DoPrimaryAction(AActor* Holder) override;
	virtual void CompletePrimaryAction(AActor* Holder) override;
	virtual void DoSecondaryAction(AActor* Holder) override;
	virtual void DoTertiaryAction(AActor* Holder) override;
	virtual void OnHand(AActor* NewOwner) override;
	virtual void OnRelease() override;
	
	/* Healing 관련 섹션 */
private:
	void PerformHealing(AActor* Holder);
	void ClearHealingTimer();
	
	UPROPERTY()
	FTimerHandle HealingTimerHandle;

	bool bIsReleasedButton = false;
	bool bIsHealing = false;
};
