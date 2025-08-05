// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PEUITestPlayerController.h"
#include "Blueprint\UserWidget.h"

void APEUITestPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BackMainMenu();
}

void APEUITestPlayerController::StartGame()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

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

void APEUITestPlayerController::PauseMenu()
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

void APEUITestPlayerController::ResumeGame()
{
	UE_LOG(LogTemp, Warning, TEXT("active"));
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("pause Remove"));
	}
	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
		UE_LOG(LogTemp, Warning, TEXT("HUD Addtov"));
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetPause(false);

}

void APEUITestPlayerController::ShowCredit()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

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

void APEUITestPlayerController::ShowHelper()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

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

void APEUITestPlayerController::BackMainMenu()
{
	ClearAllUIWidget();

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

void APEUITestPlayerController::ClearAllUIWidget()
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

	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr;
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
