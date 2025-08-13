// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PEUseableComponent.generated.h"

/*
 *	손에 들고 사용되는 액터들을 위한 컴포넌트 클래스입니다.
 *	구현해야 하는 기능:
 *	1. Hold: 액터를 손에 들 때 호출되는 함수
 *	2. Release: 액터를 손에서 놓을 때 호출되는 함수
 *	3. Use: 액터를 사용할 때 호출되는 함수(인터페이스를 통해 Owner에게 전달됨)
 */

enum class EPEEquipmentType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEUseableComponent : public UActorComponent
{
	GENERATED_BODY()

	/* Lifecycle 관련 섹션 */
public:
	UPEUseableComponent();

protected:
	virtual void BeginPlay() override;

	/* 장착 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, Category = "Useable")
	bool bIsHold;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Useable")
	void Hold();

	UFUNCTION(BlueprintCallable, Category = "Useable")
	void Release();

	UFUNCTION(BlueprintCallable, Category = "Useable")
	void DoPrimaryAction(AActor* User);
	
	UFUNCTION(BlueprintCallable, Category = "Useable")
	void CompletePrimaryAction(AActor* User);
	
	UFUNCTION(BlueprintCallable, Category = "Useable")
	void DoSecondaryAction(AActor* User);
	
	UFUNCTION(BlueprintCallable, Category = "Useable")
	void DoTertiaryAction(AActor* User);

	bool IsHolding() const;

	EPEEquipmentType GetEquipmentType() const;
};
