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

	// Create FirstPerson SkeletalMesh
	FirstPersonSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonSkeletalMesh"));
	FirstPersonSkeletalMesh->SetupAttachment(FirstPersonCameraComponent);

	// Receive Attack Component
	ReceiveAttackComponent = CreateDefaultSubobject<UPEReceiveAttackComponent>(TEXT("ReceiveAttackComponent"));
	ReceiveAttackComponent->SetHiddenInGame(false);
	ReceiveAttackComponent->SetupAttachment(RootComponent);
}

void APEHero::BeginPlay()
{
	Super::BeginPlay();
	
	if (HeroInputComponent)
	{
		HeroInputComponent->InputConfiguration();
	}
	
	// 인벤토리 아이템 드롭 델리게이트 바인딩
	OnInventoryItemDrop.AddDynamic(this, &APEHero::HandleInventoryItemDrop);
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
	 * 우선순위
	 * 1. StorableItemComponent가 있는 경우 - 인벤토리 아이템 상호작용
	 * 2. QuickSlotItemComponent가 있는 경우 - 퀵슬롯 아이템 상호작용
	 */
	if (UPEStorableItemComponent* StorableItemComponent = TargetActor->FindComponentByClass<UPEStorableItemComponent>())
	{
		if (InteractManagerComponent && InteractManagerComponent->HasInteractable())
		{
			// 인벤토리 아이템 상호작용
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

UPEUseableItemManagerComponent* APEHero::GetUseableItemManagerComponent() const
{
	return UseableItemManagerComponent;
}

void APEHero::HandEquipment(EPEEquipmentType EquipmentType)
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

void APEHero::DropHandEquipmentToWorld()
{
	if (UseableItemManagerComponent)
	{
		UseableItemManagerComponent->DropHandEquipmentToWorld();
	}
}

UPEQuickSlotManagerComponent* APEHero::GetQuickSlotManagerComponent() const
{
	return QuickSlotManagerComponent;
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

void APEHero::HandleInventoryItemDrop(FGameplayTag ItemTag, int32 DropCount)
{
	if (!InventoryManagerComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("HandleInventoryItemDrop: InventoryManagerComponent is null!"));
		return;
	}
	
	// 인벤토리 매니저의 DropItemFromInventoryByTag 함수 호출
	InventoryManagerComponent->DropItemFromInventoryByTag(DropCount, ItemTag);
	
	UE_LOG(LogTemp, Log, TEXT("HandleInventoryItemDrop: Dropped %d items with tag %s"), 
		DropCount, *ItemTag.ToString());
}

void APEHero::HandleInventoryItemUse(FGameplayTag ItemTag)
{
	UseItemByInventory(ItemTag);
	
	UE_LOG(LogTemp, Log, TEXT("HandleInventoryItemUse: Used item with tag %s"), *ItemTag.ToString());
}

void APEHero::UseItemByInventory(FGameplayTag ItemTag)
{
	// NOTE: 인벤토리에서 아이템에 마우스 오른쪽 버튼을 눌렀을 때의 액션이 이곳에 호출됩니다.
	//			ItemBase는 해당 함수를 비워둡니다.
}

bool APEHero::HasWeapon() const
{
	if (UseableItemManagerComponent)
	{
		if (UseableItemManagerComponent->GetCurrentItem())
		{
			return true;
		}
	}
	return false;
}

void APEHero::AttachWeapon(AActor* WeaponActor, FTransform Transform)
{
	if (WeaponActor)
	{
		if (FirstPersonSkeletalMesh)
		{
			FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepRelativeTransform;
			FName SocketName = FName(TEXT("weapon_r"));
			WeaponActor->SetActorRelativeTransform(Transform);
			WeaponActor->AttachToComponent(FirstPersonSkeletalMesh, Rule, SocketName);

			PlayEquipAnimation();
		}
	}
}

void APEHero::PlayEquipAnimation()
{
	float PlayRate = 1.0f;
	PlayMontageAnimation(EquipAnimMontage, PlayRate);
}

void APEHero::PlayReloadAnimation(float ReloadDelay)
{
	float AnimationLength = ReloadAnimMontage->GetPlayLength();
	float PlayRate = ReloadDelay == 0 ? 1.0f : AnimationLength / ReloadDelay;
	PlayMontageAnimation(ReloadAnimMontage, PlayRate);
}

void APEHero::PlayFireWeaponAnimation(float ShotInterval)
{
	float AnimationLength = ReloadAnimMontage->GetPlayLength();
	float PlayRate = ShotInterval == 0 ? 1.0f : AnimationLength / ShotInterval;
	PlayMontageAnimation(FireWeaponAnimMontage, PlayRate);
}

void APEHero::PlayMontageAnimation(UAnimMontage* Animation, float PlayRate)
{
	if (Animation && EquipAnimMontage)
	{
		if (UAnimInstance* AnimInstance = FirstPersonSkeletalMesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(Animation, PlayRate);
		}
	}
}
