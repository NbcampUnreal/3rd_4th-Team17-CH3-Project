#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	void InputConfiguration();
	void SetupEnhancedInput(UInputComponent* PlayerInputComponent);

	void OnInputMove(const FInputActionValue& Value);
	void OnInputLook(const FInputActionValue& Value);
	void OnInputStartSprint(const FInputActionValue& Value);
	void OnInputStopSprint(const FInputActionValue& Value);
	void OnInputStartJump(const FInputActionValue& Value);
	void OnInputStopJump(const FInputActionValue& Value);
	void OnInputToggleCrouch(const FInputActionValue& Value);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Mapping")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Configuration")
	float NormalSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Configuration")
	float SprintSpeed = 800.0f;

protected:
	ACharacter* GetOwnerCharacter();
	UCharacterMovementComponent* GetOwnerMovementComponent();
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();
};
