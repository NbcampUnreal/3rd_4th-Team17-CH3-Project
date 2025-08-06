#include "Characters/Hero/Components/PEHeroInputComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UPEHeroInputComponent::UPEHeroInputComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPEHeroInputComponent::InputConfiguration()
{
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->MaxWalkSpeed = NormalSpeed;
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
				EIC->BindAction(CrouchInputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputToggleCrouch);
			}
		}
		
	}
}

void UPEHeroInputComponent::OnInputMove(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter())
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
	if (ACharacter* Hero = GetOwnerCharacter())
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
	// TODO: 스태미너
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->MaxWalkSpeed = SprintSpeed;
	}
}

void UPEHeroInputComponent::OnInputStopSprint(const FInputActionValue& Value)
{
	if (UCharacterMovementComponent* MovementComponent = GetOwnerMovementComponent())
	{
		MovementComponent->MaxWalkSpeed = NormalSpeed;
	}
}

void UPEHeroInputComponent::OnInputStartJump(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter())
	{
		Hero->Jump();
	}
}

void UPEHeroInputComponent::OnInputStopJump(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter())
	{
		Hero->StopJumping();
	}

}

void UPEHeroInputComponent::OnInputToggleCrouch(const FInputActionValue& Value)
{
	if (ACharacter* Hero = GetOwnerCharacter())
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

ACharacter* UPEHeroInputComponent::GetOwnerCharacter()
{
	if (ACharacter* Hero = Cast<ACharacter>(GetOwner()))
	{
		return Hero;
	}
	return nullptr;
}

UCharacterMovementComponent* UPEHeroInputComponent::GetOwnerMovementComponent()
{
	if (ACharacter* Hero = GetOwnerCharacter())
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
	if (ACharacter* Hero = GetOwnerCharacter())
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
