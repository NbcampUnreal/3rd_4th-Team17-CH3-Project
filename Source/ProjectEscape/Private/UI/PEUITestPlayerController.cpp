// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PEUITestPlayerController.h"
#include "Components\ProgressBar.h"
#include "Blueprint\UserWidget.h"

void APEUITestPlayerController::BeginPlay()
{
	Super::BeginPlay();
	MaxHealth = 300;
	Health = MaxHealth;

	OnDamage.AddDynamic(this, &APEUITestPlayerController::HandleHealthChanged);

	ShowMainMenu();
}

void APEUITestPlayerController::ShowMainMenu()
{
	if (!MainMenuWidget && MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	}

	if (MainMenuWidget && !MainMenuWidget->IsInViewport())
	{
		MainMenuWidget->AddToViewport();
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		SetPause(true);
	}
}

void APEUITestPlayerController::OnClickStartGame()
{
	ClearAllWidget();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetPause(false);

}

void APEUITestPlayerController::PauseGameAndShowPauseMenu()
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

void APEUITestPlayerController::OnClickResume()
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

void APEUITestPlayerController::OnClickCredit()
{
	ClearAllWidget();

	if (!CreditWidget && CreditWidgetClass)
	{
		CreditWidget = CreateWidget<UUserWidget>(this, CreditWidgetClass);
	}

	if (CreditWidget && !CreditWidget->IsInViewport())
	{
		CreditWidget->AddToViewport();
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		SetPause(true);
	}

}

void APEUITestPlayerController::OnClickHelper()
{
	ClearAllWidget();

	if (!HelperWidget && HelperWidgetClass)
	{
		HelperWidget = CreateWidget<UUserWidget>(this, HelperWidgetClass);
	}

	if (HelperWidget && !HelperWidget->IsInViewport())
	{
		HelperWidget->AddToViewport();
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		SetPause(true);
	}

}

void APEUITestPlayerController::OnClickBackMainMenu()
{
	ClearAllWidget();

	if (!MainMenuWidget && MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
	}

	if (MainMenuWidget && !MainMenuWidget->IsInViewport())
	{
		MainMenuWidget->AddToViewport();
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		SetPause(true);
	}

}

void APEUITestPlayerController::ClearAllWidget()
{
	if (CreditWidget)
	{
		CreditWidget->RemoveFromParent();
		CreditWidget = nullptr;
	}

	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	if (HelperWidget)
	{
		HelperWidget->RemoveFromParent();
		HelperWidget = nullptr;
	}
}

float APEUITestPlayerController::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvnet, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvnet, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);

	OnDamage.Broadcast(Health, MaxHealth);



	return DamageAmount;
}
// 테스트용 이게 실제로 잘 적용될지는 모르겠음. 
void APEUITestPlayerController::HandleHealthChanged(float DCCurrentHealth, float DCMaxHealth)
{
	if (HUDWidget)
	{
		UFunction* DamageFunc = HUDWidget->FindFunction(FName("PlayDamageAnim"));
		if (DamageFunc)
		{
			HUDWidget->ProcessEvent(DamageFunc, nullptr);
		}
	}
	if (HUDWidget)
	{
		if (!HealthBar)
		{
			HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar")));
		}
		if (HealthBar)
		{
			HealthBar->SetPercent(DCCurrentHealth / DCMaxHealth);
		}
	}
}
