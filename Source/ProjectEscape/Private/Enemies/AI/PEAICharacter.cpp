// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/PEAICharacter.h"
#include "Enemies/AI/PEAIController.h"

#include "GameFramework/CharacterMovementComponent.h"

APEAICharacter::APEAICharacter()
{
	// AIController class setup
	AIControllerClass = APEAIController::StaticClass(); 
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 

	// variable initialization
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed; 
	Movement->bOrientRotationToMovement = true; 
	Movement->RotationRate = RotateSpeed; 
	Movement->AirControl = AirControl; 
}

// Called when the game starts or when spawned
void APEAICharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("AICharacter has been spawned"));
}

void APEAICharacter::SetMovementSpeed(float NewSpeed)
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement)
	{
		Movement->MaxWalkSpeed = NewSpeed; // Update the walk speed
		WalkSpeed = NewSpeed; // Update the WalkSpeed property
		UE_LOG(LogTemp, Warning, TEXT("AICharacter movement speed set to: %f"), WalkSpeed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set movement speed: Movement component not found"));
	}
}
