#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEHeroInputComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:
	ACharacter* GetOwnerCharacter();
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();
};
