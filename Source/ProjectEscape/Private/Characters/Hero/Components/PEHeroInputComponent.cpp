#include "Characters/Hero/Components/PEHeroInputComponent.h"
#include "Player/PEPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/Hero/PEHero.h"
#include "Characters/Hero/Components/PEInteractManagerComponent.h"
#include "Characters/Hero/Components/PEUseableItemManagerComponent.h"
#include "Items/Components/PEUseableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"

UPEHeroInputComponent::UPEHeroInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bIsSprint = false;
}

void UPEHeroInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		bool isMoving = !FMath::IsNearlyZero(MovementComponent->Velocity.Length());
		if (bIsSprint && isMoving)
		{
			if (!CheckCanSprintAndCommitSprint(DeltaTime))
			{
				StopSprint();
			}
		}
		else
		{
			RecoverCostWhileNotSprinting(DeltaTime);
		}
	}
}

void UPEHeroInputComponent::InputConfiguration()
{
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->MaxWalkSpeed = NormalSpeed;
		MovementComponent->NavAgentProps.bCanCrouch = bCanCrouch;
	}
}

void UPEHeroInputComponent::SetupEnhancedInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputLocalPlayerSubsystem())
	{
		Subsystem->ClearAllMappings();
		if (DefaultMappingContext)
		{
			const int32 Priority = 1;
			Subsystem->AddMappingContext(DefaultMappingContext, Priority);
		}

		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if (MoveInputAction)
			{
				EIC->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputMove);
			}
			if (LookInputAction)
			{
				EIC->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputLook);
			}
			if (SprintInputAction)
			{
				EIC->BindAction(SprintInputAction, ETriggerEvent::Started, this, &ThisClass::OnInputStartSprint);
				EIC->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputStopSprint);
			}
			if (JumpInputAction)
			{
				EIC->BindAction(JumpInputAction, ETriggerEvent::Started, this, &ThisClass::OnInputStartJump);
				EIC->BindAction(JumpInputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputStopJump);
			}
			if (CrouchInputAction)
			{
				EIC->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &ThisClass::OnInputToggleCrouch);
			}
			for (int32 Index = 0; Index < QuickSlotInputActions.Num(); Index++)
			{
				UInputAction* QuickSlotInputAction = QuickSlotInputActions[Index];
				if (QuickSlotInputAction)
				{
					// To make it easier to understand, counting starts from 1 rather than 0.
					EIC->BindAction(QuickSlotInputAction, ETriggerEvent::Started, this, &ThisClass::OnInputQuickSlotNumber, Index + 1);
				}
			}
			if (ReloadInputAction)
			{
				EIC->BindAction(ReloadInputAction, ETriggerEvent::Started, this, &ThisClass::OnInputReload);
			}
			if (InteractInputAction)
			{
				EIC->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputInteract);
			}
			if (PrimayActionInputAction)
			{
				EIC->BindAction(PrimayActionInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputPrimaryActionTriggered);
				EIC->BindAction(PrimayActionInputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputPrimaryActionCompleted);
			}
			if (SecondaryActionInputAction)
			{
				EIC->BindAction(SecondaryActionInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputSecondaryActionTriggered);
				EIC->BindAction(SecondaryActionInputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputSecondaryActionCompleted);
			}
		}
		
	}
}

void UPEHeroInputComponent::OnInputMove(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		FVector2D MoveVector = Value.Get<FVector2D>();
		if (!FMath::IsNearlyZero(MoveVector.Length()))
		{
			FVector MovementVector = FVector::ZeroVector;
			MovementVector += Hero->GetActorForwardVector() * MoveVector.X;
			MovementVector += Hero->GetActorRightVector() * MoveVector.Y;
			Hero->AddMovementInput(MovementVector);
		}
	}
}

void UPEHeroInputComponent::OnInputLook(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		FVector2D LookVector = Value.Get<FVector2D>();
		if (!FMath::IsNearlyZero(LookVector.Length()))
		{
			Hero->AddControllerYawInput(LookVector.X);
			Hero->AddControllerPitchInput(LookVector.Y);
		}
	}
}

void UPEHeroInputComponent::OnInputStartSprint(const FInputActionValue& Value)
{
	if (!bIsSprint)
	{
		if (CheckCanStartSprint())
		{
			StartSprint();
			bIsSprint = true;
		}
	}
	
}

void UPEHeroInputComponent::OnInputStopSprint(const FInputActionValue& Value)
{
	if (bIsSprint)
	{
		StopSprint();
		bIsSprint = false;
	}
}

void UPEHeroInputComponent::OnInputStartJump(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		Hero->Jump();
	}
}

void UPEHeroInputComponent::OnInputStopJump(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		Hero->StopJumping();
	}

}

void UPEHeroInputComponent::OnInputToggleCrouch(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		if (Hero->CanCrouch())
		{
			Hero->Crouch();
		}
		else
		{
			Hero->UnCrouch();
		}
	}
}

void UPEHeroInputComponent::OnInputQuickSlotNumber(const FInputActionValue& Value, int32 SlotNumber)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("SlotNumber: %d"), SlotNumber));
	}
#endif
}

void UPEHeroInputComponent::OnInputReload(const FInputActionValue& Value)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("On Input Reload")));
	}
#endif
}

void UPEHeroInputComponent::OnInputInteract(const FInputActionValue& Value)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("On Input Interact Triggered!!")));
	}
#endif
	if (APEHero* Hero = GetOwnerCharacter<APEHero>())
	{
		Hero->GetInteractManagerComponent()->TryInteract();
	}
}

void UPEHeroInputComponent::OnInputPrimaryActionTriggered(const FInputActionValue& Value)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("On Input Primary Action Triggered!!")));
	}
#endif
	if (APEHero* Hero = GetOwnerCharacter<APEHero>())
	{
		Hero->Use();
	}
}

void UPEHeroInputComponent::OnInputPrimaryActionCompleted(const FInputActionValue& Value)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::Printf(TEXT("On Input Primary Action Complete...")));
	}
#endif
}

void UPEHeroInputComponent::OnInputSecondaryActionTriggered(const FInputActionValue& Value)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, FString::Printf(TEXT("On Input Secondary Action Triggered!!")));
	}
#endif
}

void UPEHeroInputComponent::OnInputSecondaryActionCompleted(const FInputActionValue& Value)
{
#ifdef WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::Printf(TEXT("On Input Secondary Action Complete...")));
	}
#endif
}

UCharacterMovementComponent* UPEHeroInputComponent::GetOwnerMovementComponent()
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		if (UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(Hero->GetMovementComponent()))
		{
			return MovementComponent;
		}
	}
	return nullptr;
}

UEnhancedInputLocalPlayerSubsystem* UPEHeroInputComponent::GetEnhancedInputLocalPlayerSubsystem()
{
	if (ACharacter* Hero = GetOwnerCharacter<ACharacter>())
	{
		if (const APlayerController* PC = Hero->GetController<APlayerController>())
		{
			if (const ULocalPlayer* LP = PC->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					return Subsystem;
				}
			}
		}
	}
	return nullptr;
}

bool UPEHeroInputComponent::CheckCanStartSprint()
{
	if (APEPlayerState* PlayerState = GetOwnerPlayerState<APEPlayerState>())
	{
		return PlayerState->CanStartSprint();
	}
	return false;
}

bool UPEHeroInputComponent::CheckCanSprintAndCommitSprint(float DeltaTime)
{
	if (APEPlayerState* PlayerState = GetOwnerPlayerState<APEPlayerState>())
	{
		if (PlayerState->CanSprint(DeltaTime))
		{
			PlayerState->CommitSprint(DeltaTime);
			return true;
		}
	}
	return false;
}

void UPEHeroInputComponent::RecoverCostWhileNotSprinting(float DeltaTime)
{
	if (APEPlayerState* PlayerState = GetOwnerPlayerState<APEPlayerState>())
	{
		PlayerState->RecoverStamina(DeltaTime);
	}
}

void UPEHeroInputComponent::StartSprint()
{
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->MaxWalkSpeed = SprintSpeed;
	}
}

void UPEHeroInputComponent::StopSprint()
{
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->MaxWalkSpeed = NormalSpeed;
	}
}
