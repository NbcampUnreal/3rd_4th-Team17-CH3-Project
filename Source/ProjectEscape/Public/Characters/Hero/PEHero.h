#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/Interface/PEAttackable.h"
#include "GameFramework/Character.h"
#include "Interface/PEInteractManagerHandler.h"
#include "Interface/PEQuickSlotHandler.h"
#include "Interface/PEWeaponAttachable.h"
#include "PEHero.generated.h"

class UCameraComponent;
class UPEInteractManagerComponent;
class UPEReceiveAttackComponent;
class UPEInventoryManagerComponent;
class UPEUseableItemManagerComponent;
class UPEQuickSlotManagerComponent;
class UPEHeroInputComponent;

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
	UPEQuickSlotManagerComponent* GetQuickSlotManagerComponent() const;
	
	/* Inventroy 관련 섹션 */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QuickSlot, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEInventoryManagerComponent> InventoryManagerComponent;

	void InventroyDropTest(); // 인벤토리 드랍 테스트용 함수

	/* Combat 관련 섹션 */
public:
	virtual USceneComponent* GetAttackStartPoint() const override;
	virtual UPEStorableItemComponent* GetStorableItemComponent(FGameplayTag Tag) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivate))
	TObjectPtr<UPEReceiveAttackComponent> ReceiveAttackComponent;

	/* Visual and Animation 관련 섹션 */
public:
	UFUNCTION(BlueprintCallable)
	bool HasWeapon() const;

	virtual void AttachWeapon(AActor* WeaponActor) override;
	virtual void DetachWeapon(AActor* WeaponActor) override;
};
