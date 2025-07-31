// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "PEInteractableComponent.generated.h"


class IPEInteractable;
class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UPEInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	UPEInteractableComponent();

protected:
	virtual void BeginPlay() override;

protected:
	IPEInteractable* ComponentOwner;

public:	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool Interact(AActor* Interactor);

	FORCEINLINE void SetComponentOwner(IPEInteractable* NewOwner) { ComponentOwner = NewOwner; }
};

