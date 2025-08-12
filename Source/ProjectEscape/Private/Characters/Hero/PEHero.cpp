#include "Characters/Hero/PEHero.h"

#include "Camera/CameraComponent.h"
#include "Characters/Hero/Components/PEHeroInputComponent.h"
#include "Characters/Hero/Components/PEInteractManagerComponent.h"
#include "Characters/Hero/Components/PEInventoryManagerComponent.h"
#include "Characters/Hero/Components/PEQuickSlotManagerComponent.h"
#include "Characters/Hero/Components/PEUseableItemManagerComponent.h"
#include "Combat/Components/PEReceiveAttackComponent.h"
#include "Items/PEEquipmentType.h"
#include "Items/Components/PEQuickSlotItemComponent.h"
#include "Items/Components/PEStorableItemComponent.h"
#include "Items/Components/PEUseableComponent.h"
#include "Items/Interface/PEUseable.h"

APEHero::APEHero()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/* Create Components */
	HeroInputComponent = CreateDefaultSubobject<UPEHeroInputComponent>("InputComponent");

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create Interact Manager Component
	InteractManagerComponent = CreateDefaultSubobject<UPEInteractManagerComponent>(TEXT("InteractManagerComponent"));
	
	// Create Useable Item Component
	UseableItemManagerComponent = CreateDefaultSubobject<UPEUseableItemManagerComponent>(TEXT("UseableItemManagerComponent"));

	// Create Quick Slot Component
	QuickSlotManagerComponent = CreateDefaultSubobject<UPEQuickSlotManagerComponent>(TEXT("QuickSlotManagerComponent"));

	// Create Inventory Manager Component
	InventoryManagerComponent = CreateDefaultSubobject<UPEInventoryManagerComponent>(TEXT("InventoryManagerComponent"));

	// Receive Attack Component
	ReceiveAttackComponent = CreateDefaultSubobject<UPEReceiveAttackComponent>(TEXT("ReceiveAttackComponent"));
	ReceiveAttackComponent->SetHiddenInGame(false);
	ReceiveAttackComponent->SetupAttachment(RootComponent);

	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
}

void APEHero::BeginPlay()
{
	Super::BeginPlay();
	
	if (HeroInputComponent)
	{
		HeroInputComponent->InputConfiguration();
	}

	if (AIPerceptionStimuliSourceComponent)
	{
		AIPerceptionStimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());

		// �߰������� �ٸ� ������ ��� ����
		// AIPerceptionStimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Hearing>());

		AIPerceptionStimuliSourceComponent->RegisterWithPerceptionSystem();

		UE_LOG(LogTemp, Log, TEXT("Player registered for AI Sight perception"));
	}

}

void APEHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APEHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (HeroInputComponent)
	{
		HeroInputComponent->SetupEnhancedInput(PlayerInputComponent);
	}
}

void APEHero::TryInteract(AActor* TargetActor)
{
	/*
	 * �켱����
	 * 1. StorableItemComponent�� �ִ� ��� - �κ��丮 ������ ��ȣ�ۿ�
	 * 2. QuickSlotItemComponent�� �ִ� ��� - ������ ������ ��ȣ�ۿ�
	 */
	if (UPEStorableItemComponent* StorableItemComponent = TargetActor->FindComponentByClass<UPEStorableItemComponent>())
	{
		if (InteractManagerComponent && InteractManagerComponent->HasInteractable())
		{
			// �κ��丮 ������ ��ȣ�ۿ�
			InventoryManagerComponent->AddItemToInventory(StorableItemComponent);
			UE_LOG(LogTemp, Warning, TEXT("StorableItemComponent found and interacted with %s"), *GetNameSafe(TargetActor));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No interactable component found for %s"), *GetNameSafe(TargetActor));
		}
	}
	else if (UPEQuickSlotItemComponent* QuickSlotItemComponent = TargetActor->FindComponentByClass<UPEQuickSlotItemComponent>())
	{
		QuickSlotItemComponent->OnItemPickedUp();
		QuickSlotManagerComponent->SetQuickSlotItem(QuickSlotItemComponent->GetEquipmentType(), TargetActor);
		UE_LOG(LogTemp, Warning, TEXT("QuickSlotItemComponent found and set for %s"), *GetNameSafe(TargetActor));

		// ��ȣ�ۿ� �� ��� �ڵ� ����
		EPEEquipmentType EquipmentType = QuickSlotItemComponent->GetEquipmentType();
		if (QuickSlotManagerComponent->ContainWeaponType(EquipmentType))
		{
			HandEquipment(EquipmentType);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("TryInteract called on %s with target %s"), *GetName(), *GetNameSafe(TargetActor));
	
}

UPEInteractManagerComponent* APEHero::GetInteractManagerComponent() const
{
	return InteractManagerComponent; 
}

void APEHero::DoPrimaryAction()
{
	if (UseableItemManagerComponent)
	{
		UseableItemManagerComponent->DoPrimaryActionCurrentItem(this);
		UE_LOG(LogTemp, Warning, TEXT("Primary Action with UseableComponent: %s"), *UseableItemManagerComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UseableComponent is not initialized!"));
	}
}

void APEHero::CompletePrimaryAction()
{
	if (UseableItemManagerComponent)
	{
		UseableItemManagerComponent->CompletePrimaryActionCurrentItem(this);
		UE_LOG(LogTemp, Warning, TEXT("Primary Action Complete with UseableComponent: %s"), *UseableItemManagerComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UseableComponent is not initialized!"));
	}
}

void APEHero::DoSecondaryAction()
{
	if (UseableItemManagerComponent)
	{
		UseableItemManagerComponent->DoSecondaryActionCurrentItem(this);
		UE_LOG(LogTemp, Warning, TEXT("Secondary Action called with UseableComponent: %s"), *UseableItemManagerComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UseableComponent is not initialized!"));
	}
}

void APEHero::DoTertiaryAction()
{
	if (UseableItemManagerComponent)
	{
		UseableItemManagerComponent->DoTertiaryActionCurrentItem(this);
		UE_LOG(LogTemp, Warning, TEXT("Tertiary Action called with UseableComponent: %s"), *UseableItemManagerComponent->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UseableComponent is not initialized!"));
	}
}

void APEHero::HandEquipment(EPEEquipmentType EquipmentType)
{
	// Todo: ���������� ���� �������� ���� ���� �տ� ����
	if (!QuickSlotManagerComponent || !UseableItemManagerComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Required components are not initialized!"));
		return;
	}
	
	if (AActor* HandItem = QuickSlotManagerComponent->SelectEquipment(EquipmentType))
	{
		// �����ۿ��� ���� UPEUseableComponent�� ã��
		if (UPEUseableComponent* UseableComponent = HandItem->FindComponentByClass<UPEUseableComponent>())
		{
			// �������� �տ� ��� �ִ� ���·� ����
			UseableItemManagerComponent->SetHandItem(UseableComponent);
			
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

void APEHero::InventroyDropTest()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->ItemDropTest();
	}
}

USceneComponent* APEHero::GetAttackStartPoint() const
{
	return FirstPersonCameraComponent;
}

UPEStorableItemComponent* APEHero::GetStorableItemComponent(FGameplayTag Tag) const
{
	return InventoryManagerComponent->GetItemByTag(Tag);
}

