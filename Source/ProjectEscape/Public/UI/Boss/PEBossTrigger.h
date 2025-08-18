// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PEBossTrigger.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBoxTrigger);

UCLASS()
class PROJECTESCAPE_API APEBossTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	APEBossTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	FOnBoxTrigger OnBoxTrigger;


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bIsBind;
};
