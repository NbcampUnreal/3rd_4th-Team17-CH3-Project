// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PEUITestPlayerController.h"
#include "Blueprint\UserWidget.h"

void APEUITestPlayerController::BeginPlay()
{
	Super::BeginPlay();

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
