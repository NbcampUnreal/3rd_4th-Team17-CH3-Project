#include "Player/PEPlayerState.h"
#include "Player/PEPlayerController.h"
#include "Core/PEGameModeBase.h"

APEPlayerState::APEPlayerState()
{
	HealthPoint = MaxHealthPoint;
	Stamina = MaxStamina;
}

void APEPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (APEPlayerController* PEPlayerController = Cast<APEPlayerController>(GetPlayerController()))
	{
		OnChangeHealthPoint.AddUObject(PEPlayerController, &APEPlayerController::OnChangeHealthPoint);
		OnChangeStamina.AddUObject(PEPlayerController, &APEPlayerController::OnChangeStamina);
	}
}

void APEPlayerState::SetHealthPoint(float NewValue)
{
	if (FMath::IsNearlyEqual(NewValue, HealthPoint))
	{
		return;
	}

	float OldValue = HealthPoint;
	HealthPoint = FMath::Clamp(NewValue, 0, MaxHealthPoint);
	OnChangeHealthPoint.Broadcast(OldValue, HealthPoint, MaxHealthPoint);

	if (HealthPoint <= 0)
	{
		
		if (APlayerController* Controller = GetPlayerController())
		{
			if (UWorld* World = Controller->GetWorld())
			{
				if (APEGameModeBase* PEGameModeBase = Cast<APEGameModeBase>(World->GetAuthGameMode()))
				{
					PEGameModeBase->GameOver(Controller);
				}
			}
		}
	}
}

void APEPlayerState::SetMaxHealthPoint(float NewValue)
{
	if (NewValue > 0)
	{
		if (FMath::IsNearlyEqual(NewValue, MaxHealthPoint))
		{
			return;
		}

		MaxHealthPoint = NewValue;
		SetHealthPoint(HealthPoint);
	}
}

void APEPlayerState::IncreaseHealthPoint(float Value)
{
	SetHealthPoint(HealthPoint + Value);
}

void APEPlayerState::ReduceHealthPoint(float Value)
{
	SetHealthPoint(HealthPoint - Value);
}

void APEPlayerState::SetStamina(float NewValue)
{
	if (FMath::IsNearlyEqual(NewValue, Stamina))
	{
		return;
	}

	Stamina = FMath::Clamp(NewValue, 0, MaxStamina);
	OnChangeStamina.Broadcast(Stamina, MaxStamina);
}

void APEPlayerState::SetMaxStamina(float NewValue)
{
	if (NewValue > 0)
	{
		if (FMath::IsNearlyEqual(NewValue, MaxStamina))
		{
			return;
		}

		MaxStamina = NewValue;
		SetStamina(Stamina);
	}
}

void APEPlayerState::IncreaseStamina(float Value)
{
	SetStamina(Stamina + Value);
}

void APEPlayerState::ReduceStamina(float Value)
{
	SetStamina(Stamina - Value);
}

bool APEPlayerState::CanStartSprint() const
{
	// Requires stamina to run for at least 0.1 seconds.
	float MinimumCostToSprint = SprintCostPerSecond * 0.1f;
	return (Stamina >= MinimumCostToSprint);
}

bool APEPlayerState::CanSprint(float DeltaTime) const
{
	float CostToSprint = SprintCostPerSecond * DeltaTime;
	return (Stamina >= CostToSprint);
}

void APEPlayerState::CommitSprint(float DeltaTime)
{
	float CostToSprint = SprintCostPerSecond * DeltaTime;
	ReduceStamina(CostToSprint);
}

void APEPlayerState::RecoverStamina(float DeltaTime)
{
	float RecoveredSprint = SprintRestorePerSecond * DeltaTime;
	IncreaseStamina(RecoveredSprint);
}

// Function For Test. TODO: Remove.
void APEPlayerState::ReduceHealthPoint_FOR_TEST(float Value)
{
	ReduceHealthPoint(Value);
}

// Function For Test. TODO: Remove.
void APEPlayerState::ReduceStamina_FOR_TEST(float Value)
{
	ReduceStamina(Value);
}
