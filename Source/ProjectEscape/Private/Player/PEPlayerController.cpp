#include "Player/PEPlayerController.h"
#include "Player\PEPlayerState.h"
#include "Components\ProgressBar.h"
#include "Blueprint/UserWidget.h"


void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHUD();

}

void APEPlayerController::OnChangeHealthPoint(float HealthPoint, float MaxHealthPoint)
{
	PlayDamageAnimOfHUDWidget();
	ChangeHealthBar(HealthPoint, MaxHealthPoint);
}

void APEPlayerController::OnChangeStamina(float Stamina, float MaxStamina)
{
	ChangeStaminaBar(Stamina,MaxStamina);

}

void APEPlayerController::PlayDamageAnimOfHUDWidget()
{

	if (HUDWidget)
	{
		UFunction* DamageFunc = HUDWidget->FindFunction(FName("PlayDamageAnim"));
		if (DamageFunc)
		{
			HUDWidget->ProcessEvent(DamageFunc, nullptr);
		}
	}
}

void APEPlayerController::ChangeHealthBar(float HealthPoint, float MaxHealthPoint)
{
	if (HUDWidget)
	{
		UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar")));

		if (HealthBar)
		{
			HealthBar->SetPercent(HealthPoint / MaxHealthPoint);
			UE_LOG(LogTemp, Warning, TEXT("hp : %.f , %.f"), HealthPoint, MaxHealthPoint);
		}
	}
}

void APEPlayerController::ChangeStaminaBar(float Stamina, float MaxStamina)
{
	if (HUDWidget)
	{
		UProgressBar* StaminaBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("StaminaBar")));

		if (StaminaBar)
		{
			StaminaBar->SetPercent(Stamina / MaxStamina);
		}
	}
}

void APEPlayerController::ShowHUD()
{
	if (!HUDWidget && HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	}

	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
		SetPause(false);
	}
}

void APEPlayerController::PauseGameAndShowPauseMenu()
{
	if (!PauseMenuWidget && PauseMenuWidgetClass)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(this, PauseMenuWidgetClass);
	}

	if (PauseMenuWidget && !PauseMenuWidget->IsInViewport())
	{
		PauseMenuWidget->AddToViewport();
	}

	if (HUDWidget && HUDWidget->IsInViewport())
	{
		HUDWidget->RemoveFromParent();
	}

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	SetPause(true);
}

void APEPlayerController::OnClickResume()
{
	ClearAllWidget();

	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetPause(false);
}

void APEPlayerController::ClearAllWidget()
{
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}
}

