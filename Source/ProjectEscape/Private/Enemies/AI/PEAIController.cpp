// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AI/PEAIController.h"
#include "NavigationSystem.h"
#include "Enemies/AI/PEAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

APEAIController::APEAIController()
{
	// AIPerception ������Ʈ�� �����ϰ� �� AI�� ���̴� ������ ����
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	//�þ� ����(Sight) ������ ���� ȯ�� ���� ������Ҹ� �����ϰ�, ���� Ž�� �ݰ�� �þ߰� �� �ν� ���� �ð� ���� �ֿ� ���� ���� ����.
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius; 
	SightConfig->LoseSightRadius = LoseSightRadius; 
	SightConfig->PeripheralVisionAngleDegrees = SightAngle; 
	SightConfig->SetMaxAge(SightDuration);

	// AI�� �þ߰� ��(Enemies), �߸�(Neutrals), �Ʊ�(Friendlies) ��θ� �ν��ϵ��� ����. ���� ���� ����
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; 
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// ������ ������ �þ� ���� �������(SightConfig)�� AI Perception�� ����
	AIPerception->ConfigureSense(*SightConfig); 
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation()); 
}

void APEAIController::BeginPlay()
{
	Super::BeginPlay();

	if(AIPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("���ε�"));
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &APEAIController::OnPerceptionUpdated);
	}
	
	GetWorld()->GetTimerManager().SetTimer(RandomMoveTimer, this, &APEAIController::MoveToRandomLocation, PatrolCycle, true);
}

void APEAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RandomMoveTimer);
	}
}

void APEAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
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

	bool bFoundLocation = NavSystem->GetRandomPointInNavigableRadius(MyPawn->GetActorLocation(), MoveRadius, RandomLocation);

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
	APawn* MyPawn = GetPawn();
	if (!MyPawn) return;
	FVector DisplayLocation = MyPawn->GetActorLocation() + FVector(0, 0, 100);

	if (Stimulus.WasSuccessfullySensed())
	{
		DrawDebugString(GetWorld(),
			DisplayLocation + FVector(0, 0, 100),
			FString::Printf(TEXT("�߰�: %s"), *Actor->GetName()),
			nullptr,
			FColor::Green,
			2.0f,
			true);

	}
	else
	{
		DrawDebugString(GetWorld(),
			DisplayLocation + FVector(0, 0, 100),
			FString::Printf(TEXT("��ħ: %s"), *Actor->GetName()),
			nullptr,
			FColor::Red,
			2.0f,
			true);
	
	}
}