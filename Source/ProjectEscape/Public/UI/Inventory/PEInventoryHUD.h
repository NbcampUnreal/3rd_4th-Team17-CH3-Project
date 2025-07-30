#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryType.h"
#include "PEInventoryHUD.generated.h"

class UUniformGridPanel;
class UUserWidget;
class UActorComponent;
class UPEInventoryRangeWeaponSlot;
class UPEInventoryMeleeWeaponSlot;
class UPEInventoryQuickItemSlot;

UCLASS()
class PROJECTESCAPE_API UPEInventoryHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPEInventoryHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void InitInventoryUI(FInventoryInfo InInventoryInfo);

	UFUNCTION(BlueprintCallable)
	void DropItemFromBagToLand(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SwapItemInBag(int32 Index, int32 OhterIndex);

	//UFUNCTION(BlueprintCallable)
	//void UnequipWeaponAndDropToLand(EInventoryWeaponCategory WeaponCategory);

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
	TObjectPtr<UUniformGridPanel> BagSlotUniformGrid;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryRangeWeaponSlot> MainWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryRangeWeaponSlot> SubWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryMeleeWeaponSlot> MeleeWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickItemSlot> HealItemSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickItemSlot> GrenadeSlot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> BagSlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagRowCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagColCount;

	FInventoryInfo InventoryInfo;


private:
	void CreateEmptyBagSlots();
	void ResetBagSlots();
	void InitBagSlots();

	void ResetSlot(int32 Index);
	void SwapSlot(int32 Index, int32 OtherIndex);

	TMap<int32, UUserWidget*> BagSlots;
};
