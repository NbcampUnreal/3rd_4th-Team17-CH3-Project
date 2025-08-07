#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PEPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFloatStatusChanged, /*Current*/ float, /*Max*/ float)

UCLASS()
class PROJECTESCAPE_API APEPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APEPlayerState();

	virtual void BeginPlay() override;

	/* Status Delegates, Getters, Setters */
	FOnFloatStatusChanged OnChangeHealthPoint;
	FOnFloatStatusChanged OnChangeStamina;

	float GetHealthPoint() const { return HealthPoint; }
	float GetMaxHealthPoint() const { return MaxHealthPoint; }

	float GetStamina() const { return Stamina; }
	float GetMaxStamina() const { return MaxStamina; }


	void SetHealthPoint(float NewValue);
	void SetMaxHealthPoint(float NewValue);
	void IncreaseHealthPoint(float Value);

	//테스트용
	UFUNCTION(BlueprintCallable)
	void ReduceHealthPoint(float Value);

	void SetStamina(float NewValue);
	void SetMaxStamina(float NewValue);
	void IncreaseStamina(float Value);

	//테스트용
	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Value);

protected:
	/* Status */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float MaxHealthPoint = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float HealthPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float MaxStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float Stamina;
};
