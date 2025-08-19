#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/Interface/PEAttackable.h"
#include "GameFramework/Character.h"
#include "Interface/PEInteractManagerHandler.h"
#include "Interface/PEQuickSlotHandler.h"
#include "UI/Inventory/PEInventoryType.h"
#include "Interface/PEWeaponAttachable.h"
#include "PEHero.generated.h"

struct FPEEquipmentInfo;
class UAnimMontage;

// 인벤토리 리스트 구조체
USTRUCT(BlueprintType)
struct PROJECTESCAPE_API FInventoryList
{
	GENERATED_BODY()
public:
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemDrop, FGameplayTag, ItemTag, int32, DropCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, FInventoryInfo&, InventoryInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUse, FGameplayTag, ItemTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotEquipmentDrop, FGameplayTag, EquipmentTag);

class UCameraComponent;
class UPEInteractManagerComponent;
class UPEReceiveAttackComponent;
class UPEInventoryManagerComponent;
class UPEUseableItemManagerComponent;
class UPEQuickSlotManagerComponent;
class UPEHeroInputComponent;
class UAIPerceptionStimuliSourceComponent;

UCLASS()
class PROJECTESCAPE_API APEHero : public ACharacter, public IPEInteractManagerHandler, public IPEQuickSlotHandler, public IPEAttackable, public IPEWeaponAttachable
{
	GENERATED_BODY()

public:
	APEHero();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPEHeroInputComponent> HeroInputComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

protected:
	virtual void BeginPlay() override;

	/* Interact 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEInteractManagerComponent> InteractManagerComponent;

public:
	UPEInteractManagerComponent* GetInteractManagerComponent() const;
	virtual void TryInteract(AActor* TargetActor) override;

	/* 장비 사용 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UseItem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEUseableItemManagerComponent> UseableItemManagerComponent;

public:
	virtual void DoPrimaryAction();
	virtual void CompletePrimaryAction();
	virtual void DoSecondaryAction();
	virtual void DoTertiaryAction();
	virtual UPEUseableItemManagerComponent* GetUseableItemManagerComponent() const;
	
	/* Quick Slot 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEQuickSlotManagerComponent> QuickSlotManagerComponent;

public:
	virtual void HandEquipment(EPEEquipmentType EquipmentType) override;
	virtual void DropHandEquipmentToWorld() override;

	UFUNCTION()
	virtual void HandleDropEquipmentToWorld(FGameplayTag EquipmentTag);
	
	UPEQuickSlotManagerComponent* GetQuickSlotManagerComponent() const;
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnQuickSlotEquipmentDrop OnQuickSlotEquipmentDrop;
	
	/* Inventroy 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QuickSlot, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEInventoryManagerComponent> InventoryManagerComponent;

	void InventroyDropTest(); // 인벤토리 드랍 테스트용 함수
	
	UFUNCTION()
	void HandleInventoryItemDrop(FGameplayTag ItemTag, int32 DropCount);

	UFUNCTION()
	void HandleInventoryItemUse(FGameplayTag ItemTag);

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryItemDrop OnInventoryItemDrop;
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory Events")
	FOnInventoryItemUse OnInventoryItemUse;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItemByInventory(FGameplayTag ItemTag);

	UPEInventoryManagerComponent* GetInventoryManagerComponent() const;

	/* Combat 관련 섹션 */
public:
	virtual USceneComponent* GetAttackStartPoint() const override;
	virtual UPEStorableItemComponent* GetStorableItemComponent(FGameplayTag Tag) const override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivate))
	TObjectPtr<UPEReceiveAttackComponent> ReceiveAttackComponent;

	/* UI 관련 섹션 */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	FInventoryInfo CurrentInventoryList;

	void SetInventoryBagInfo(const FInventoryInfo& InventoryInfo);
	void SetQuickSlotInfo(const FInventoryInfo& InventoryInfo);
	void BroadcastInventoryChanged();
	void BroadCastEquipmentChanged(FPEEquipmentInfo& EquipmentInfo);
	
	// AI Perception Stimulus Source 컴포넌트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComponent;

	/* Visual and Animation Sections*/
public:
	UFUNCTION(BlueprintCallable)
	bool HasWeapon() const;
	void PlayEquipAnimation();
	void PlayReloadAnimation(float ReloadDelay);
	void PlayFireWeaponAnimation(float ShotInterval);

	virtual void AttachWeapon(AActor* WeaponActor, FTransform Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> FirstPersonSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> EquipAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> ReloadAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> FireWeaponAnimMontage;

protected:
	void PlayMontageAnimation(UAnimMontage* Animation, float PlayRate);
};
