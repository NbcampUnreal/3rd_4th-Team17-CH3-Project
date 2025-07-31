// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/PEAIController.h"
#include "NavigationSystem.h"
#include "Enemies/AI/PEAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

APEAIController::APEAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f; // Set the sight radius for the AI
	SightConfig->LoseSightRadius = 2000.0f; // Set the lose sight radius
	SightConfig->PeripheralVisionAngleDegrees = 90.0f; // Set the peripheral vision angle
	SightConfig->SetMaxAge(5.0f); // Set the maximum age of the perception data

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // Allow detection of enemies
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // Allow detection of neutrals
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // Allow detection of friendlies

	AIPerception->ConfigureSense(*SightConfig); // Configure the perception component with the sight configuration
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation()); // Set the dominant sense to sight
}

void APEAIController::BeginPlay()
{
	Super::BeginPlay();

	if(AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &APEAIController::OnPerceptionUpdated); // Bind the perception update event
	}

	// Set a timer to move to a random location every 3 seconds
	GetWorld()->GetTimerManager().SetTimer(RandomMoveTimer, this, &APEAIController::MoveToRandomLocation, 3.0f, true);
}

void APEAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		// Custom logic for AIController when it possesses a pawn
		UE_LOG(LogTemp, Warning, TEXT("AIController has possessed: %s"), *InPawn->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController has no pawn to possess."));
	}
}

void APEAIController::MoveToRandomLocation()
{
	APawn* MyPawn = GetPawn();

	if (!MyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController has no pawn to move."));
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Navigation system not found."));
		return;
	}

	FNavLocation RandomLocation; // Variable to hold the random navigable location

	bool bFoundLocation = NavSystem->GetRandomPointInNavigableRadius(MyPawn->GetActorLocation(), MoveRadius, RandomLocation); // Get a random navigable point within the specified radius

	if (bFoundLocation)
	{
		// Move the AI pawn to the random location
		MoveToLocation(RandomLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("AIController moving to random location: %s"), *RandomLocation.Location.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a random navigable location within radius: %f"), MoveRadius);
	}
}

void APEAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		DrawDebugString(GetWorld(),
			Actor->GetActorLocation() + FVector(0, 0, 100),
			FString::Printf(TEXT("발견: %s"), *Actor->GetName()),
			nullptr,
			FColor::Green,
			2.0f,
			true);
		// Implement logic for when the AI perceives an actor
	}
	else
	{
		DrawDebugString(GetWorld(),
			Actor->GetActorLocation() + FVector(0, 0, 100),
			FString::Printf(TEXT("놓침: %s"), *Actor->GetName()),
			nullptr,
			FColor::Red,
			2.0f,
			true);
		// Implement logic for when the AI loses sight of an actor
	}
}