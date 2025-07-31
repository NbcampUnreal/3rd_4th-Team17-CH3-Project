// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/PEAICharacter.h"
#include "Enemies/AI/PEAIController.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APEAICharacter::APEAICharacter()
{
	AIControllerClass = APEAIController::StaticClass(); // Set this character to use the custom AI controller class
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // Automatically possess this AI character when placed in the world or spawned 

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed; // Set the default walk speed for the AI character
	Movement->bOrientRotationToMovement = true; // Make the character face the direction of movement
	Movement->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // 회전 속도 부드럽게 설정
	Movement->AirControl = 0.2f; // Set air control for the character
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
