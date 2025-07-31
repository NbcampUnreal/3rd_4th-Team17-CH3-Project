#include "UI\HUD\PEHUD.h"
#include "Blueprint\UserWidget.h"
#include "Components\TextBlock.h"
#include "Components\ProgressBar.h"
#include "GameFramework\PlayerController.h"

APEHUD::APEHUD()
{
	HUDWidgetInstance = nullptr;
}

void APEHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowPEHUD();
	UE_LOG(LogTemp, Warning, TEXT("showpehud"));
}

void APEHUD::ShowPEHUD()
{
	UE_LOG(LogTemp, Warning, TEXT("hud?"));
	if (HUDWidgetClass && !HUDWidgetInstance)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		UE_LOG(LogTemp, Warning, TEXT("pcused?"));
		if (!PC) return;
		
		HUDWidgetInstance = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
			PC->SetInputMode(FInputModeGameOnly());
			PC->bShowMouseCursor = false;
		}

	}
}

void APEHUD::UpdateHealth(float CurrentHP, float MaxHP)
{
	if (!HUDWidgetInstance) return;

	if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidgetInstance->GetWidgetFromName(TEXT("HealthBar"))))
	{
		const float Percent = MaxHP > 0 ? CurrentHP / MaxHP : 0.f;
		HealthBar->SetPercent(Percent);
	}

}

void APEHUD::UpdateScore(int32 score)
{
	if (!HUDWidgetInstance) return;

	if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidgetInstance->GetWidgetFromName(TEXT("ScoreText"))))
	{
		ScoreText->SetText(FText::AsNumber(score));
	}

}


