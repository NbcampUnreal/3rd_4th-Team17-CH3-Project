#include "Player/PEPlayerController.h"
#include "Player\PEPlayerState.h"
#include "Components\ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/Inventory/PEInventoryHUD.h"


void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHUD();

	OnWeaponInfoBroadcast.AddDynamic(this, &APEPlayerController::OnChangeWeaponInfo);

	if (APEPlayerState* PS = GetPlayerState<APEPlayerState>())
	{
		ChangeHealthBar(PS->GetHealthPoint(), PS->GetMaxHealthPoint());
		ChangeStaminaBar(PS->GetStamina(), PS->GetMaxStamina());
	}

	if (InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
		if (UPEInventoryHUD* InventoryHUD = Cast<UPEInventoryHUD>(InventoryWidget))
		{
			// TODO: Connecting the components required for this Widget
			InventoryHUD->SetupComponentReference(nullptr, nullptr);
		}
	}
}

void APEPlayerController::OnChangeHealthPoint(float HealthPoint, float MaxHealthPoint)
{
	// 체력이 올라갈때도 변화가 일어남. 델리케이트로 빼고난뒤에 제거해야함.
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
// 데미지를 받았을때 실행
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

// 상대방에게 데미지를 주었을때 실행
void APEPlayerController::PlayHitMarkerAnimOfHUDWIdget()
{
	if (HUDWidget)
	{
		UFunction* HitMarkerFunc = HUDWidget->FindFunction(FName("PlayHitMarkerAnim"));
		if (HitMarkerFunc)
		{
			HUDWidget->ProcessEvent(HitMarkerFunc, nullptr);
		}
	}
}

// 상대방을 죽였을때 실행
void APEPlayerController::PlayKillMarkerAnimOfHUDWidget()
{
	if (HUDWidget)
	{
		UFunction* KillMarkerFunc = HUDWidget->FindFunction(FName("PlayKillMarkerAnim"));
		if (KillMarkerFunc)
		{
			HUDWidget->ProcessEvent(KillMarkerFunc, nullptr);
		}
	}
}

// 총을 쐈을때 실행
void APEPlayerController::PlayAimAnimOfHUDWidget()
{
	if (HUDWidget)
	{
		UFunction* AimFunc = HUDWidget->FindFunction(FName("PlayAimAnim"));
		if (AimFunc)
		{
			HUDWidget->ProcessEvent(AimFunc, nullptr);
		}
	}
}

// 총을 쐈을때 실행
void APEPlayerController::OnChangeAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (HUDWidget)
	{
		if (UTextBlock* AmmoText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("AmmoText")))
		{
			AmmoText->SetText(FText::FromString(FString::Printf(TEXT("Ammo : %d / %d"), CurrentAmmo, MaxAmmo)));
		}
	}
}

void APEPlayerController::OnChangeWeaponInfo(FPEEquipmentInfo& EquipmentInfo)
{
	if (HUDWidget)
	{
		if (UTextBlock* WeaponText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("WeaponText")))
		{
			WeaponText->SetText(FText::FromName(EquipmentInfo.EquipmentName));
		}
		
		if (UImage* WeaponImage = Cast<UImage>(HUDWidget->GetWidgetFromName("WeaponImage")))
		{
			if (EquipmentInfo.EquipmentIcon)
			{
				WeaponImage->SetBrushFromTexture(EquipmentInfo.EquipmentIcon);
			}
			else
			{
				WeaponImage->SetBrush(FSlateBrush());
			}
		}

		if (UTextBlock* AmmoText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("AmmoText")))
		{
			AmmoText->SetText(FText::FromString(EquipmentInfo.AmmoCount));
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
	}
}

