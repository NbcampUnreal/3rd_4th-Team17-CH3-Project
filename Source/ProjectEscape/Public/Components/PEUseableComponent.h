// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEUseableComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEUseableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPEUseableComponent();

protected:
	virtual void BeginPlay() override;

	/* 장착 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, Category = "Useable")
	TObjectPtr<AActor> CurrentHolder;

	UPROPERTY(VisibleAnywhere, Category = "Useable")
	bool bIsHold;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Useable")
	void Hold(AActor* Holder);

	UFUNCTION(BlueprintCallable, Category = "Useable")
	void Release(AActor* Holder);

	UFUNCTION(BlueprintCallable, Category = "Useable")
	void Use(AActor* User);
	
	FORCEINLINE void SetOwner(AActor* NewOwner) { CurrentHolder = NewOwner; }
};
