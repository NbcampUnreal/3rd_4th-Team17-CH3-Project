#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PEPlayerController.generated.h"

struct FInventoryInfo;

UCLASS()
class PROJECTESCAPE_API APEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PauseGameAndShowPauseMenu();

	UFUNCTION(BlueprintCallable)
	void OnClickResume();

	void ShowHUD();
	void ClearAllWidget();

	UFUNCTION()
	void OnChangeHealthPoint(float HealthPoint, float MaxHealthPoint);

	UFUNCTION()
	void OnChangeStamina(float HealthPoint, float MaxHealthPoint);

	UFUNCTION()
	void OnInventoryAndQuickSlotUpdate(FInventoryInfo& InInventoryInfo);

	void PlayDamageAnimOfHUDWidget();
	void ChangeHealthBar(float HealthPoint, float MaxHealthPoint);
	void ChangeStaminaBar(float Stamina, float MaxStamina);

	void ToggleInventoryWidget();
	void OpenInventoryWidget();
	void CloseInventoryWidget();
	bool IsOpenInventory() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UUserWidget* HUDWidget;
	UUserWidget* PauseMenuWidget;
	UUserWidget* InventoryWidget;
};
