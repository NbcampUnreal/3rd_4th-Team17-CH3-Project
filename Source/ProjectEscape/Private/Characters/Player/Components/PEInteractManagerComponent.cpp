// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Components/PEInteractManagerComponent.h"
#include "Items/Components/PEInteractableComponent.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Characters/Player/Interface/PETryInteract.h"

UPEInteractManagerComponent::UPEInteractManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	InteractionRange = 300.0f;
	CurrentInteractable = nullptr;
	OwnerPawn = nullptr;
	InteractAction = nullptr;
}

void UPEInteractManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 소유자 Pawn 캐시
	OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PEInteractManagerComponent: Owner is not a Pawn!"));
		return;
	}

	// 입력 바인딩 설정
	SetupInputBindings();
}

void UPEInteractManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Todo: 상호작용 가능 UI 호출
}

void UPEInteractManagerComponent::TryInteract()
{
	// 상호작용 시도 시 Ray 캐스팅 실행
	CheckAndSetForInteractable();
	
	if (CurrentInteractable)
	{
		// 상호작용 실행
		if (CurrentInteractable->Interact(OwnerPawn))
		{
			if (IPETryInteract* TryInteractInterface = Cast<IPETryInteract>(OwnerPawn))
			{
				TryInteractInterface->TryInteract(CurrentInteractable->GetOwner());
				UE_LOG(LogTemp, Warning, TEXT("Interacted with: %s"), *CurrentInteractable->GetOwner()->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Owner Pawn does not implement IPETryInteract interface!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No interactable object found"));
	}
}

void UPEInteractManagerComponent::CheckAndSetForInteractable()
{
	if (!OwnerPawn)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// 카메라 위치와 방향 가져오기
	FVector CameraLocation;
	FRotator CameraRotation;
	
	// Pawn의 Control Rotation 사용 (화면 중앙 방향)
	OwnerPawn->GetActorEyesViewPoint(CameraLocation, CameraRotation);
	
	// Ray 시작점과 끝점 계산
	FVector StartLocation = CameraLocation;
	FVector ForwardVector = CameraRotation.Vector();
	FVector EndLocation = StartLocation + (ForwardVector * InteractionRange);

	// Ray Cast 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerPawn);
	QueryParams.bTraceComplex = false;
	
	FHitResult HitResult;
	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams
	);

	// 디버그 라인 그리기 (개발 중에만)
	#if WITH_EDITOR
	DrawDebugLine(
		World,
		StartLocation,
		EndLocation,
		bHit ? FColor::Green : FColor::Red,
		false,
		0.1f,
		0,
		1.0f
	);
	#endif

	// 이전 상호작용 대상 초기화
	UPEInteractableComponent* NewInteractable = nullptr;

	if (bHit && HitResult.GetActor())
	{
		// Hit된 액터에서 PEInteractableComponent 찾기
		NewInteractable = HitResult.GetActor()->FindComponentByClass<UPEInteractableComponent>();
	}

	// 상호작용 대상이 변경되었을 때 로그 출력
	if (CurrentInteractable != NewInteractable)
	{
		if (NewInteractable)
		{
			UE_LOG(LogTemp, Log, TEXT("Found interactable: %s"), *NewInteractable->GetOwner()->GetName());
		}
		else if (CurrentInteractable)
		{
			UE_LOG(LogTemp, Log, TEXT("Lost interactable: %s"), *CurrentInteractable->GetOwner()->GetName());
		}
		
		CurrentInteractable = NewInteractable;
	}
}

//Todo: 해당 코드를 없애고, 캐릭터가 상호작용 함수 자체를 호출하도록 변경
void UPEInteractManagerComponent::SetupInputBindings()
{
	if (!OwnerPawn)
	{
		return;
	}

	// Enhanced Input Component 가져오기
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerPawn->InputComponent))
	{
		if (InteractAction)
		{
			// 상호작용 액션 바인딩
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UPEInteractManagerComponent::OnInteractPressed);
			UE_LOG(LogTemp, Log, TEXT("PEInteractManagerComponent: Input bindings set up successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PEInteractManagerComponent: InteractAction is not set!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PEInteractManagerComponent: Enhanced Input Component not found!"));
	}
}

void UPEInteractManagerComponent::SetInteractAction(UInputAction* NewInteractAction)
{
	InteractAction = NewInteractAction;
	
	// InteractAction이 설정된 후 입력 바인딩 재설정
	if (OwnerPawn)
	{
		SetupInputBindings();
	}
}

void UPEInteractManagerComponent::OnInteractPressed(const FInputActionValue& Value)
{
	// 상호작용 실행
	TryInteract();
}
