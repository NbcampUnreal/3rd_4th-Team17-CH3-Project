// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PEUIPlayerController.generated.h"

class UUserWidget;

UCLASS()
class PROJECTESCAPE_API APEUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	void StartGame();
	UFUNCTION(BlueprintCallable)
	void PauseMenu();
	UFUNCTION(BlueprintCallable)
	void ResumeGame();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidget;
	UPROPERTY()
	UUserWidget* MainMenuWidget;
	UPROPERTY()
	UUserWidget* PauseMenuWidget;

	bool bIsPause = false;


};
