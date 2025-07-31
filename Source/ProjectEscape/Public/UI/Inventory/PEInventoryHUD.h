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

	UFUNCTION(BlueprintCallable)
	void InitInventoryUI(FInventoryInfo InInventoryInfo);

	UFUNCTION(BlueprintCallable)
	void DropItemFromBagToLand(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SwapItemInBag(int32 Index, int32 OhterIndex);

	//UFUNCTION(BlueprintCallable)
	//void UnequipWeaponAndDropToLand(EInventoryQuickSlotCategory WeaponCategory);

	//UFUNCTION(BlueprintCallable)
	//void AddItemToQuickSlot();

	//UFUNCTION(BlueprintCallable)
	//void RemoveItemFromQuickSlot();

	// Interface, To be confirmed.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActorComponent> IntenvoryBagComponent;

	// Interface, To be confirmed.
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActorComponent> EquipmentQuickSlotComponent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryBagSlot_Widget> BagSlotWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlot_Widget> QuickSlotWidget;

	FInventoryInfo InventoryInfo;
};
