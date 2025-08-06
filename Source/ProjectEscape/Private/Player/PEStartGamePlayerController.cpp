#include "Player/PEStartGamePlayerController.h"
#include "Blueprint/UserWidget.h"

void APEStartGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
}

void APEStartGamePlayerController::OnClickCredit()
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

void APEStartGamePlayerController::OnClickHelper()
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

void APEStartGamePlayerController::OnClickBackMainMenu()
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

void APEStartGamePlayerController::ClearAllWidget()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}
	if (CreditWidget)
	{
		CreditWidget->RemoveFromParent();
		CreditWidget = nullptr;
	}
	if (HelperWidget)
	{
		HelperWidget->RemoveFromParent();
		HelperWidget = nullptr;
	}
}

void APEStartGamePlayerController::ShowMainMenu()
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
