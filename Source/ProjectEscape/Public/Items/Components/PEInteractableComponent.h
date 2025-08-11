// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "PEInteractableComponent.generated.h"


class IPEInteractable;
class UBoxComponent;

/**
 * UPEInteractableComponent는 상호작용 가능한 액터에 부착되어 상호작용 기능을 제공하는 컴포넌트입니다.
 * 이 컴포넌트는 IPEInteractable 인터페이스를 구현하여 상호작용 기능을 제공합니다.
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTESCAPE_API UPEInteractableComponent : public UBoxComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:	
	UPEInteractableComponent();

protected:
	virtual void BeginPlay() override;

	/* Interact 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TScriptInterface<IPEInteractable> ComponentOwner;

public:	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool Interact(AActor* Interactor);

	void SetComponentOwnerInterface(UObject* NewOwner);
};

