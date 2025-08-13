#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryType.h"
#include "PEInventoryHUD.generated.h"

class UUserWidget;
class UActorComponent;
class UPEInventoryBagSlot_Widget;
class UPEInventoryQuickSlot_Widget;

UCLASS()
class PROJECTESCAPE_API UPEInventoryHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	// Interface, To be confirmed.
	void SetupComponentReference(UActorComponent* InIntenvoryBagComponent, UActorComponent* InEquipmentQuickSlotComponent, AActor* InHero);

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryUI(UPARAM(ref) FInventoryInfo& InInventoryInfo);

	UFUNCTION(BlueprintCallable)
	void DropItemFromBagToLand(const FGameplayTag& Tag, const int32& Count);

	UFUNCTION(BlueprintCallable)
	void UnequipWeaponAndDropToLand(FGameplayTag WeaponTag);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromQucikSlotAndDropToLand(FGameplayTag WeaponTag);

	UFUNCTION(BlueprintCallable)
	void AddItemFromBagToQuickSlot(FGameplayTag ItemTag, int32 FromSlotIndex);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromQuickSlotToBag(FGameplayTag ItemTag, int32 ToSlotIndex);

	// Interface, To be confirmed.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActorComponent> IntenvoryBagComponent;

	// Interface, To be confirmed.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActorComponent> EquipmentQuickSlotComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Hero;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryBagSlot_Widget> BagSlotWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlot_Widget> QuickSlotWidget;
};
