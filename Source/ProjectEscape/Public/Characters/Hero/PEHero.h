#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combat/Interface/PEAttackable.h"
#include "GameFramework/Character.h"
#include "Interface/PEInteractManagerHandler.h"
#include "Interface/PEQuickSlotHandler.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "PEHero.generated.h"

class UCameraComponent;
class UPEInteractManagerComponent;
class UPEReceiveAttackComponent;
class UPEInventoryManagerComponent;
class UPEUseableItemManagerComponent;
class UPEQuickSlotManagerComponent;
class UPEHeroInputComponent;

UCLASS()
class PROJECTESCAPE_API APEHero : public ACharacter, public IPEInteractManagerHandler, public IPEQuickSlotHandler, public IPEAttackable
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

	/* Interact ���� ���� */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEInteractManagerComponent> InteractManagerComponent;

public:
	UPEInteractManagerComponent* GetInteractManagerComponent() const;
	virtual void TryInteract(AActor* TargetActor) override;

	/* ��� ��� ���� ���� */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UseItem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEUseableItemManagerComponent> UseableItemManagerComponent;

public:
	virtual void DoPrimaryAction();
	virtual void CompletePrimaryAction();
	virtual void DoSecondaryAction();
	virtual void DoTertiaryAction();
	
	/* Quick Slot ���� ���� */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEQuickSlotManagerComponent> QuickSlotManagerComponent;

public:
	virtual void HandEquipment(EPEEquipmentType EquipmentType) override;
	
	/* Inventroy ���� ���� */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = QuickSlot, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPEInventoryManagerComponent> InventoryManagerComponent;

	void InventroyDropTest(); // �κ��丮 ��� �׽�Ʈ�� �Լ�

	/* Combat ���� ���� */
public:
	virtual USceneComponent* GetAttackStartPoint() const override;
	virtual UPEStorableItemComponent* GetStorableItemComponent(FGameplayTag Tag) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivate))
	TObjectPtr<UPEReceiveAttackComponent> ReceiveAttackComponent;

	// AI Perception Stimulus Source ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> AIPerceptionStimuliSourceComponent;

};
