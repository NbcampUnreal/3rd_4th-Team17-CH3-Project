// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEUIDamage.generated.h"

class UWidgetComponent;
class UUserWidget;
class UTextBlock;

UCLASS()
class PROJECTESCAPE_API APEUIDamage : public AActor
{
	GENERATED_BODY()
	
public:	

	APEUIDamage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DamageWidgetclass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 0.0f;

	void UpdateDamageUI();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FTimerHandle TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
	UWidgetComponent* WidgetComp;

	void DestroySelf();

};
