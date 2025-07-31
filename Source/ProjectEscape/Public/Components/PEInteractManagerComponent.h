// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PEInteractManagerComponent.generated.h"

class UPEInteractableComponent;
class UInputAction;

/*
 *	Interact를 실행시킬 액터에 부착하여 사용하는 컴포넌트 클래스입니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTESCAPE_API UPEInteractManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPEInteractManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 상호작용 가능한 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float InteractionRange = 300.0f;

	// 상호작용 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	// 현재 감지된 상호작용 컴포넌트
	UPROPERTY()
	TObjectPtr<UPEInteractableComponent> CurrentInteractable;

	// 소유자 캐릭터 캐시
	UPROPERTY()
	TObjectPtr<class APawn> OwnerPawn;

public:
	// 상호작용 실행 함수
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();

	// Ray 캐스팅을 통한 상호작용 대상 검사
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void CheckForInteractable();

	// 현재 상호작용 가능한 대상이 있는지 확인
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool HasInteractable() const { return CurrentInteractable != nullptr; }

	// 상호작용 범위 설정
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetInteractionRange(float NewRange) { InteractionRange = NewRange; }

	// InteractAction 설정
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetInteractAction(UInputAction* NewInteractAction);

	// 입력 바인딩 설정
	UFUNCTION(BlueprintCallable, Category = "Input")
	void SetupInputBindings();

protected:
	// 상호작용 입력 처리 함수
	void OnInteractPressed(const FInputActionValue& Value);
};
