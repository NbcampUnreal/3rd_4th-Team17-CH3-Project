#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "PEHeroInputComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UCharacterMovementComponent;
struct FInputActionValue;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UPEHeroInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPEHeroInputComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InputConfiguration();
	void SetupEnhancedInput(UInputComponent* PlayerInputComponent);

	void OnInputMove(const FInputActionValue& Value);
	void OnInputLook(const FInputActionValue& Value);
	void OnInputStartSprint(const FInputActionValue& Value);
	void OnInputStopSprint(const FInputActionValue& Value);
	void OnInputStartJump(const FInputActionValue& Value);
	void OnInputStopJump(const FInputActionValue& Value);
	void OnInputToggleCrouch(const FInputActionValue& Value);
	void OnInputQuickSlotNumber(const FInputActionValue& Value, int32 SlotNumber);
	void OnInputReload(const FInputActionValue& Value);
	void OnInputPrimaryActionTriggered(const FInputActionValue& Value);
	void OnInputPrimaryActionCompleted(const FInputActionValue& Value);
	void OnInputSecondaryActionTriggered(const FInputActionValue& Value);
	void OnInputSecondaryActionCompleted(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> SprintInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> CrouchInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TArray<TObjectPtr<UInputAction>> QuickSlotInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> ReloadInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> PrimayActionInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Action")
	TObjectPtr<UInputAction> SecondaryActionInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Mapping")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Configuration")
	float NormalSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Configuration")
	float SprintSpeed = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Configuration")
	bool bCanCrouch = true;

	/* movement state control */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input|MovementState")
	bool bIsSprint = false;

protected:
	template<class T>
	T* GetOwnerCharacter();

	template<class T>
	T* GetOwnerPlayerController();

	template<class T>
	T* GetOwnerPlayerState();

	UCharacterMovementComponent* GetOwnerMovementComponent();
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();

	bool CheckCanStartSprint();
	bool CheckCanSprintAndCommitSprint(float DeltaTime);
	void RecoverCostWhileNotSprinting(float DeltaTime);
	void StartSprint();
	void StopSprint();
};

template<class T>
T* UPEHeroInputComponent::GetOwnerCharacter()
{
	if (T* Hero = Cast<T>(GetOwner()))
	{
		return Hero;
	}
	return nullptr;
}

template<class T>
T* UPEHeroInputComponent::GetOwnerPlayerController()
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		if (T* PC = Cast<T>(Hero->GetController()))
		{
			return PC;
		}
	}
	return nullptr;
}

template<class T>
T* UPEHeroInputComponent::GetOwnerPlayerState()
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		if (T* PS = Cast<T>(Hero->GetPlayerState()))
		{
			return PS;
		}
	}
	return nullptr;
}

