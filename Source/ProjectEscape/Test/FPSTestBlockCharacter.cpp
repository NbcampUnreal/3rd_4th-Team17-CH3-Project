// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestBlockCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ProjectEscape/Public/PEEquipmentType.h"
#include "ProjectEscape/Public/Components/PEQuickSlotItemComponent.h"
#include "ProjectEscape/Public/Components/PEQuickSlotManagerComponent.h"
#include "ProjectEscape/Public/Components/PEUseableItemManagerComponent.h"
#include "Engine/LocalPlayer.h"
#include "Interface/PEQuickSlotItem.h"
#include "ProjectEscape/Public/Interface/PEUseable.h"
#include "ProjectEscape/Public/Components/PEInteractManagerComponent.h"
#include "ProjectEscape/Public/Components/PEUseableComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFPSTestBlockCharacter

AFPSTestBlockCharacter::AFPSTestBlockCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Create Interact Manager Component
	InteractManagerComponent = CreateDefaultSubobject<UPEInteractManagerComponent>(TEXT("InteractManagerComponent"));

	// Create Useable Item Component
	UseableItemManagerComponent = CreateDefaultSubobject<UPEUseableItemManagerComponent>(TEXT("UseableItemManagerComponent"));

	// Create Quick Slot Component
	QuickSlotManagerComponent = CreateDefaultSubobject<UPEQuickSlotManagerComponent>(TEXT("QuickSlotManagerComponent"));
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSTestBlockCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AFPSTestBlockCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSTestBlockCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSTestBlockCharacter::Look);

		// Interacting - InteractManagerComponent가 자체적으로 바인딩을 처리하므로 여기서는 InteractAction만 설정
		if (InteractManagerComponent && InteractAction)
		{
			// InteractManagerComponent에 InteractAction 설정
			InteractManagerComponent->SetInteractAction(InteractAction);
		}

		//Use
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &AFPSTestBlockCharacter::Use);

		// Hand Equipment
		EnhancedInputComponent->BindAction(HandPrimaryAction, ETriggerEvent::Triggered, this, &AFPSTestBlockCharacter::HandPrimary);
		EnhancedInputComponent->BindAction(HandSecondaryAction, ETriggerEvent::Triggered, this, &AFPSTestBlockCharacter::HandSecondary);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFPSTestBlockCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFPSTestBlockCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AFPSTestBlockCharacter::TryInteract(AActor* TargetActor)
{
	//Todo: 우선순위 설정 필요 (인벤토리 > 퀵슬롯)

	if (UPEQuickSlotItemComponent* QuickSlotItemComponent = TargetActor->FindComponentByClass<UPEQuickSlotItemComponent>())
	{
		QuickSlotItemComponent->OnItemPickedUp();
		QuickSlotManagerComponent->SetQuickSlotItem(QuickSlotItemComponent->GetEquipmentType(), TargetActor);
		UE_LOG(LogTemp, Warning, TEXT("QuickSlotItemComponent found and set for %s"), *GetNameSafe(TargetActor));

		// 상호작용 한 장비 자동 장착
		EPEEquipmentType EquipmentType = QuickSlotItemComponent->GetEquipmentType();
		if (QuickSlotManagerComponent->ContainWeaponType(EquipmentType))
		{
			HandEquipment(EquipmentType);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("TryInteract called on %s with target %s"), *GetName(), *GetNameSafe(TargetActor));
	
}

UPEInteractManagerComponent* AFPSTestBlockCharacter::GetInteractManagerComponent() const
{
	return InteractManagerComponent; 
}

void AFPSTestBlockCharacter::Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Use called"));
	if (UseableItemManagerComponent)
	{
		UseableItemManagerComponent->UseCurrentItem(this);
		UE_LOG(LogTemp, Warning, TEXT("Use called with UseableComponent: %s"), *UseableItemManagerComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UseableComponent is not initialized!"));
	}
}

void AFPSTestBlockCharacter::HandEquipment(EPEEquipmentType EquipmentType)
{
	// Todo: 퀵슬롯으로 부터 아이템을 받은 다음 손에 장착
	if (!QuickSlotManagerComponent || !UseableItemManagerComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Required components are not initialized!"));
		return;
	}
	
	if (AActor* HandItem = QuickSlotManagerComponent->SelectEquipment(EquipmentType))
	{
		// 아이템에서 직접 UPEUseableComponent를 찾기
		if (UPEUseableComponent* UseableComponent = HandItem->FindComponentByClass<UPEUseableComponent>())
		{
			// 아이템을 손에 들고 있는 상태로 설정
			UseableItemManagerComponent->SetHandItem(UseableComponent);
			
			// 인터페이스가 구현되어 있다면 OnHand 호출
			if (IPEUseable* UseableInterface = Cast<IPEUseable>(HandItem))
			{
				UseableInterface->OnHand(this);
			}
			
			UE_LOG(LogTemp, Warning, TEXT("HandEquipment: %s is now in hand"), *HandItem->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HandEquipment: %s does not have UPEUseableComponent!"), *GetNameSafe(HandItem));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No equipment found for type: %d"), static_cast<int32>(EquipmentType));
	}
}

void AFPSTestBlockCharacter::HandPrimary()
{
	HandEquipment(EPEEquipmentType::Primary);
}

void AFPSTestBlockCharacter::HandSecondary()
{
	HandEquipment(EPEEquipmentType::Secondary);
}
