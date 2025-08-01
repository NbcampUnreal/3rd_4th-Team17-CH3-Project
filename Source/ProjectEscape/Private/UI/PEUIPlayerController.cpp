// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PEUIPlayerController.h"
#include "Blueprint\UserWidget.h"

void APEUIPlayerController::BeginPlay()
{

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			SetInputMode(FInputModeUIOnly());
			bShowMouseCursor = true;
			SetPause(true);

		}

	}

}

void APEUIPlayerController::StartGame()
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

void APEUIPlayerController::PauseMenu()
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

void APEUIPlayerController::ResumeGame()
{
	UE_LOG(LogTemp, Warning, TEXT("123"));
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;
	}
	if (HUDWidget && !HUDWidget->IsInViewport())
	{
		HUDWidget->AddToViewport();
	}

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	SetPause(false);

}
