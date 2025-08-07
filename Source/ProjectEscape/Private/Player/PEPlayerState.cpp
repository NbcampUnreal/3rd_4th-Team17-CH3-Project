#include "Player/PEPlayerState.h"
#include "Player/PEPlayerController.h"

APEPlayerState::APEPlayerState()
{
	HealthPoint = MaxHealthPoint;
	Stamina = MaxStamina;
}

void APEPlayerState::BeginPlay()
{
	Super::BeginPlay();
	APEPlayerController* PEPlayerController = Cast<APEPlayerController>(GetPlayerController());
	OnChangeHealthPoint.AddUObject(PEPlayerController, &APEPlayerController::OnChangeHealthPoint);
	OnChangeStamina.AddUObject(PEPlayerController, &APEPlayerController::OnChangeStamina);

}

void APEPlayerState::SetHealthPoint(float NewValue)
{
	HealthPoint = FMath::Clamp(NewValue, 0, MaxHealthPoint);
	OnChangeHealthPoint.Broadcast(HealthPoint, MaxHealthPoint);

	if (HealthPoint <= 0)
	{
		// TODO: Implement Hero character death logic
	}
}

void APEPlayerState::SetMaxHealthPoint(float NewValue)
{
	if (NewValue > 0)
	{
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
	Stamina = FMath::Clamp(NewValue, 0, MaxStamina);
	OnChangeStamina.Broadcast(Stamina, MaxStamina);
}

void APEPlayerState::SetMaxStamina(float NewValue)
{
	if (NewValue > 0)
	{
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

