// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/PEAIController.h"
#include "Characters/Enemies/AI/PEAICharacter.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void APEAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false); // ������ �� �ʱ�ȭ
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);

		StartBehaviorTree();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("������ ������Ʈ �������� ����"));
	}

	if(AIPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("���ε�"));
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &APEAIController::OnPerceptionUpdated);
	}
	
}

void APEAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

}

void APEAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Component is not set."));
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

void APEAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if(Actor != PlayerPawn)
	{
		return; // Ignore perception updates for non-player actors
	}

	APawn* MyPawn = GetPawn();
	if (!MyPawn)
	{
		return;
	}

	FVector LastKnownLocation = Actor->GetActorLocation();

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ProjectedLocation;
	if (NavSys && NavSys->ProjectPointToNavigation(LastKnownLocation, ProjectedLocation)) {
		LastKnownLocation = ProjectedLocation.Location;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
	#ifdef WITH_EDITOR
		FVector DisplayLocation = MyPawn->GetActorLocation() + FVector(0, 0, 100);
		DrawDebugString(GetWorld(),
			DisplayLocation + FVector(0, 0, 100), // 100 ��ŭ ���ϰ� �ٽ� ���ϴ� ������ �����
			FString::Printf(TEXT("�߰�: %s"), *Actor->GetName()),
			nullptr,
			FColor::Green,
			2.0f,
			true);
	#endif

		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor); // �����忡 Ÿ�� ���� ����
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), true);
		BlackboardComp->SetValueAsVector(TEXT("TargetLastKnownLocation"), LastKnownLocation);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);
		GetWorld()->GetTimerManager().ClearTimer(InvestigateTimerHandle);

	}
	else
	{
	#ifdef WITH_EDITOR
		FVector DisplayLocation = MyPawn->GetActorLocation() + FVector(0, 0, 100);
		DrawDebugString(GetWorld(),
			DisplayLocation + FVector(0, 0, 100), // 100 ��ŭ ���ϰ� �ٽ� ���ϴ� ������ �����
			FString::Printf(TEXT("��ħ: %s"), *Actor->GetName()),
			nullptr,
			FColor::Red,
			2.0f,
			true);
	#endif
	
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false);
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), true);
		BlackboardComp->SetValueAsVector(TEXT("TargetLastKnownLocation"), LastKnownLocation);

		GetWorld()->GetTimerManager().SetTimer(
			InvestigateTimerHandle,
			this,
			&APEAIController::EndInvestigating,
			InvestigateDuration,
			false);
			

	}
}

void APEAIController::EndInvestigating()
{
	BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);
	BlackboardComp->ClearValue(TEXT("TargetActor"));
	BlackboardComp->ClearValue(TEXT("TargetLastKnownLocation"));
}