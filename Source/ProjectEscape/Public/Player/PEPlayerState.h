#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PEPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFloatStatusChanged, /*Current*/ float, /*Max*/ float)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnFloatStatusChangedOldAndNew, /*Old*/ float, /*Current*/ float, /*Max*/ float)

UCLASS()
class PROJECTESCAPE_API APEPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APEPlayerState();

	virtual void BeginPlay() override;

	/* Status Delegates, Getters, Setters */
	FOnFloatStatusChangedOldAndNew OnChangeHealthPoint;
	FOnFloatStatusChanged OnChangeStamina;

	float GetHealthPoint() const { return HealthPoint; }
	float GetMaxHealthPoint() const { return MaxHealthPoint; }

	float GetStamina() const { return Stamina; }
	float GetMaxStamina() const { return MaxStamina; }

	void SetHealthPoint(float NewValue);
	void SetMaxHealthPoint(float NewValue);
	void IncreaseHealthPoint(float Value);
	void ReduceHealthPoint(float Value);

	void SetStamina(float NewValue);
	void SetMaxStamina(float NewValue);
	void IncreaseStamina(float Value);
	void ReduceStamina(float Value);
	
	/* Secandary Operations */
	bool CanStartSprint() const;
	bool CanSprint(float DeltaTime) const;
	void CommitSprint(float DeltaTime);
	void RecoverStamina(float DeltaTime);

	// Function For Test. TODO: Remove.
	UFUNCTION(BlueprintCallable)
	void ReduceHealthPoint_FOR_TEST(float Value);

	// Function For Test. TODO: Remove.
	UFUNCTION(BlueprintCallable)
	void ReduceStamina_FOR_TEST(float Value);

protected:
	/* Status */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float MaxHealthPoint = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float HealthPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status")
	float MaxStamina = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Status")
	float Stamina;

	/* Configuration */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status|Cost")
	float SprintCostPerSecond = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status|Cost")
	float SprintRestorePerSecond = 20;
};
