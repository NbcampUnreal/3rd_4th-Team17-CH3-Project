// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PEUITestPlayerController.generated.h"

class UUserWidget;


UCLASS()
class PROJECTESCAPE_API APEUITestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	void OnClickStartGame();
	UFUNCTION(BlueprintCallable)
	void PauseMenu();
	UFUNCTION(BlueprintCallable)
	void OnClickResume();
	UFUNCTION(BlueprintCallable)
	void OnClickCredit();
	UFUNCTION(BlueprintCallable)
	void OnClickHelper();
	UFUNCTION(BlueprintCallable)
	void OnClickBackMainMenu();
	UFUNCTION(BlueprintCallable)
	void ClearAllWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> CreditWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HelperWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget;
	UPROPERTY()
	UUserWidget* MainMenuWidget;
	UPROPERTY()
	UUserWidget* PauseMenuWidget;
	UPROPERTY()
	UUserWidget* CreditWidget;
	UPROPERTY()
	UUserWidget* HelperWidget;

	bool bIsPause = false;
};
