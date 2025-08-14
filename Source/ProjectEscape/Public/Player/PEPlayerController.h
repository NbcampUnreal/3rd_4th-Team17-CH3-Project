#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Items/Weapons/FPEEquipmentInfo.h"
#include "Items/PEEquipmentType.h"
#include "PEPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponInfoBroadcast,FPEEquipmentInfo&, EquipmentInfo);

struct FInventoryInfo;
struct FGameResult;

UCLASS()
class PROJECTESCAPE_API APEPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FOnWeaponInfoBroadcast OnWeaponInfoBroadcast;

	UFUNCTION(BlueprintCallable)
	void PauseGameAndShowPauseMenu();

	UFUNCTION(BlueprintCallable)
	void OnClickResume();

	void ShowHUD();
	void ClearAllWidget();

	UFUNCTION()
	void OnChangeHealthPoint(float OldValue, float HealthPoint, float MaxHealthPoint);

	UFUNCTION()
	void OnChangeStamina(float HealthPoint, float MaxHealthPoint);

	UFUNCTION()
	void OnInventoryAndQuickSlotUpdate(FInventoryInfo& InInventoryInfo);

	UFUNCTION()
	void OnChangeWeaponInfo(FPEEquipmentInfo& EquipmentInfo);

	UFUNCTION()
	void OnChangeTotalScore(int32 TotalScore);

	UFUNCTION()
	void OnChangeMissionInfo(FText MissionInfo);

	void PlayDamageAnimOfHUDWidget();
	void PlayHitMarkerAnimOfHUDWIdget();
	void PlayKillMarkerAnimOfHUDWidget();
	void PlayAimAnimOfHUDWidget();

	void ChangeHealthBar(float HealthPoint, float MaxHealthPoint);
	void ChangeStaminaBar(float Stamina, float MaxStamina);

	void ToggleInventoryWidget();
	void OpenInventoryWidget();
	void CloseInventoryWidget();
	bool IsOpenInventory() const;

	void ShowGameOverWidget(FGameResult GameResult);
	void ShowGameClearWidget(FGameResult GameResult);

	void OnEquipmentEmptyHand();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> GameClearWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget;

	UPROPERTY()
	UUserWidget* PauseMenuWidget;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	UPROPERTY()
	UUserWidget* GameOverWidget;

	UPROPERTY()
	UUserWidget* GameClearWidget;
};
