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
	// AIPerception 컴포넌트를 생성하고 이 AI에 붙이는 역할을 수행
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	//시야 감지(Sight) 설정을 위한 환경 센스 구성요소를 생성하고, 각종 탐지 반경과 시야각 및 인식 지속 시간 등의 주요 설정 값을 지정.
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius; 
	SightConfig->LoseSightRadius = LoseSightRadius; 
	SightConfig->PeripheralVisionAngleDegrees = SightAngle; 
	SightConfig->SetMaxAge(SightDuration);

	// AI의 시야가 적(Enemies), 중립(Neutrals), 아군(Friendlies) 모두를 인식하도록 설정. 추후 수정 예정
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; 
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; 
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// 위에서 설정한 시야 감지 구성요소(SightConfig)를 AI Perception에 적용
	AIPerception->ConfigureSense(*SightConfig); 
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation()); 

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void APEAIController::BeginPlay()
{
	Super::BeginPlay();

	if(BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("CanSeeTarget"), false); // 블랙보드 값 초기화
		BlackboardComp->SetValueAsBool(TEXT("IsInvestigating"), false);

		StartBehaviorTree();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("블랙보드 컴포넌트 생성되지 않음"));
	}

	if(AIPerception)
	{
		UE_LOG(LogTemp, Warning, TEXT("바인딩"));
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
			DisplayLocation + FVector(0, 0, 100), // 100 만큼 더하고 다시 더하는 이유가 뭘까요
			FString::Printf(TEXT("발견: %s"), *Actor->GetName()),
			nullptr,
			FColor::Green,
			2.0f,
			true);
	#endif

		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor); // 블랙보드에 타겟 액터 설정
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
			DisplayLocation + FVector(0, 0, 100), // 100 만큼 더하고 다시 더하는 이유가 뭘까요
			FString::Printf(TEXT("놓침: %s"), *Actor->GetName()),
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