#include "Player/PEPlayerController.h"
#include "Player\PEPlayerState.h"
#include "Components\ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryHUD.h"


void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHUD();

	if (APEPlayerState* PS = GetPlayerState<APEPlayerState>())
	{
		ChangeHealthBar(PS->GetHealthPoint(), PS->GetMaxHealthPoint());
		ChangeStaminaBar(PS->GetStamina(), PS->GetMaxStamina());
	}

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

void APEPlayerController::OnInventoryAndQuickSlotUpdate(FInventoryInfo& InInventoryInfo)
{
	if (InventoryWidget)
	{
		if (UPEInventoryHUD* InventoryHUD = Cast<UPEInventoryHUD>(InventoryWidget))
		{
			InventoryHUD->UpdateInventoryUI(InInventoryInfo);
		}
	}
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
	if (MaxHealthPoint <= 0)
	{
		return;
	}
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
	if (MaxStamina <= 0)
	{
		return;
	}
	if (HUDWidget)
	{
		UProgressBar* StaminaBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("StaminaBar")));

		if (StaminaBar)
		{
			StaminaBar->SetPercent(Stamina / MaxStamina);
		}
	}
}

void APEPlayerController::ToggleInventoryWidget()
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		CloseInventoryWidget();
	}
	else
	{
		OpenInventoryWidget();
	}
}

void APEPlayerController::OpenInventoryWidget()
{
	if (!InventoryWidget && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
	}

	if (InventoryWidget && !InventoryWidget->IsInViewport())
	{
		InventoryWidget->AddToViewport();
		bShowMouseCursor = true;
	}
}

void APEPlayerController::CloseInventoryWidget()
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
}

bool APEPlayerController::IsOpenInventory() const
{
	if (InventoryWidget && InventoryWidget->IsInViewport())
	{
		return true;
	}
	else
	{
		return false;
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

	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
		InventoryWidget = nullptr;
	}
}

