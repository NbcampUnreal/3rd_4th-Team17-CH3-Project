#include "Player/PEPlayerController.h"
#include "Player\PEPlayerState.h"
#include "Components\ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryHUD.h"
#include "Core/PEGameStateBase.h"
#include "Components/TextBlock.h"
#include "Characters/Hero/PEHero.h"


void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowHUD();

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
			InventoryHUD->SetupComponentReference(nullptr, nullptr, GetPawn());
		}
	}

	// PEHero에 접근하여 인벤토리 변경 델리게이트 구독
	if (APEHero* Hero = Cast<APEHero>(GetPawn()))
	{
		Hero->OnInventoryChanged.AddDynamic(this, &APEPlayerController::OnInventoryAndQuickSlotUpdate);
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

void APEPlayerController::ShowGameOverWidget(FGameResult GameResult)
{
	if (GameOverWidget == nullptr && GameOverWidgetClass)
	{
		GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	}

	if (GameOverWidget && !GameOverWidget->IsInViewport())
	{
		GameOverWidget->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}

	if (GameOverWidget)
	{
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("TotalScore"))))
		{
			int32 TotalScore = GameResult.TotalScore;

			FFormatNamedArguments Args;
			Args.Add(TEXT("Score"), FText::AsNumber(TotalScore));

			FText FormatText = FText::Format(NSLOCTEXT("GameOverWidget", "TotalScoreFormat", "Total Score : {Score}"), Args);

			TotalScoreText->SetText(FormatText);
		}

		if (UTextBlock* KillCountText = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("KillCount"))))
		{
			int32 KillCount = GameResult.KillCount;

			FFormatNamedArguments Args;
			Args.Add(TEXT("KillCount"), FText::AsNumber(KillCount));

			FText FormatText = FText::Format(NSLOCTEXT("GameOverWidget", "TotalScoreFormat", "Enemies Killed : {KillCount}"), Args);

			KillCountText->SetText(FormatText);
		}

		if (UTextBlock* RunTimeText = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("RunTime"))))
		{
			int32 RunTime = GameResult.GameRunTime;

			FFormatNamedArguments Args;
			Args.Add(TEXT("RunTime"), FText::AsNumber(RunTime));

			FText FormatText = FText::Format(NSLOCTEXT("GameOverWidget", "TotalScoreFormat", "Run Time : {RunTime}"), Args);

			RunTimeText->SetText(FormatText);
		}

		if (UTextBlock* DamageDealtText = Cast<UTextBlock>(GameOverWidget->GetWidgetFromName(TEXT("DamageDealt"))))
		{
			int32 DamageDealt = GameResult.DamageDealt;

			FFormatNamedArguments Args;
			Args.Add(TEXT("DamageDealt"), FText::AsNumber(DamageDealt));

			FText FormatText = FText::Format(NSLOCTEXT("GameOverWidget", "TotalScoreFormat", "Damage Dealt : {DamageDealt}"), Args);

			DamageDealtText->SetText(FormatText);
		}

	}
}

void APEPlayerController::ShowGameClearWidget(FGameResult GameResult)
{
	if (GameClearWidget == nullptr && GameClearWidgetClass)
	{
		GameClearWidget = CreateWidget<UUserWidget>(this, GameClearWidgetClass);
	}

	if (GameClearWidget && !GameClearWidget->IsInViewport())
	{
		GameClearWidget->AddToViewport();
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}

	if (GameClearWidget)
	{
		if (UTextBlock* TotalScoreText = Cast<UTextBlock>(GameClearWidget->GetWidgetFromName(TEXT("TotalScore"))))
		{
			int32 TotalScore = GameResult.TotalScore;

			FFormatNamedArguments Args;
			Args.Add(TEXT("Score"), FText::AsNumber(TotalScore));

			FText FormatText = FText::Format(NSLOCTEXT("GameClearWidget", "TotalScoreFormat", "Total Score : {Score}"), Args);

			TotalScoreText->SetText(FormatText);
		}

		if (UTextBlock* KillCountText = Cast<UTextBlock>(GameClearWidget->GetWidgetFromName(TEXT("KillCount"))))
		{
			int32 KillCount = GameResult.KillCount;

			FFormatNamedArguments Args;
			Args.Add(TEXT("KillCount"), FText::AsNumber(KillCount));

			FText FormatText = FText::Format(NSLOCTEXT("GameClearWidget", "TotalScoreFormat", "Enemies Killed : {KillCount}"), Args);

			KillCountText->SetText(FormatText);
		}

		if (UTextBlock* RunTimeText = Cast<UTextBlock>(GameClearWidget->GetWidgetFromName(TEXT("RunTime"))))
		{
			int32 RunTime = GameResult.GameRunTime;

			FFormatNamedArguments Args;
			Args.Add(TEXT("RunTime"), FText::AsNumber(RunTime));

			FText FormatText = FText::Format(NSLOCTEXT("GameClearWidget", "TotalScoreFormat", "Run Time : {RunTime}"), Args);

			RunTimeText->SetText(FormatText);
		}

		if (UTextBlock* DamageDealtText = Cast<UTextBlock>(GameClearWidget->GetWidgetFromName(TEXT("DamageDealt"))))
		{
			int32 DamageDealt = GameResult.DamageDealt;

			FFormatNamedArguments Args;
			Args.Add(TEXT("DamageDealt"), FText::AsNumber(DamageDealt));

			FText FormatText = FText::Format(NSLOCTEXT("GameClearWidget", "TotalScoreFormat", "Damage Dealt : {DamageDealt}"), Args);

			DamageDealtText->SetText(FormatText);
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

	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromParent();
	}
}
