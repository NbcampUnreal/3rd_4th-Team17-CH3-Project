// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PEHUD.generated.h"

class UUserWidget;

UCLASS()
class PROJECTESCAPE_API APEHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	APEHUD();

	virtual void BeginPlay() override;

	void ShowPEHUD();

	void UpdateHealth(float CurrentHP, float MaxHP);

	void UpdateScore(int32 score);

protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* HUDWidgetInstance;

};
