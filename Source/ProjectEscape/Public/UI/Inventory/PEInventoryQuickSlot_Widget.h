#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryType.h"
#include "PEInventoryQuickSlot_Widget.generated.h"

class UTextBlock;
class UImage;
class UPaperSprite;
class UTexture2D;
class UPEInventoryHUD;

class UPEInventoryQuickSlotWidget_RangeWeapon;
class UPEInventoryQuickSlotWidget_MeleeWeapon;
class UPEInventoryQuickSlotWidget_UsableItem;

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickSlot_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	void CustomOnInitialized(UPEInventoryHUD* Parent);
	void UpdateInventoryUI(FInventoryInfo& InInventoryInfo);

	void SetParentWidget(UPEInventoryHUD* Parent)
	{
		ParentWidget = Parent;
	}

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlotWidget_RangeWeapon> MainWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlotWidget_RangeWeapon> SubWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlotWidget_MeleeWeapon> MeleeWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlotWidget_UsableItem> HealItemSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPEInventoryQuickSlotWidget_UsableItem> GrenadeSlot;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPEInventoryHUD> ParentWidget;
};

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickSlot_SlotBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void ResetSlot() {}

	void SetParentWidget(UPEInventoryHUD* Parent)
	{
		ParentWidget = Parent;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsValid() const { return IsVaildSlot; }

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetSlotTag() const { return SlotTag; }

	UFUNCTION(BlueprintCallable)
	void SetSlotTag(FGameplayTag InSlotTag) { SlotTag = InSlotTag; }

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetSlotCategoryTag() const { return SlotCategoryTag; }

	UFUNCTION(BlueprintCallable)
	void SetSlotCategoryTag(FGameplayTag InSlotTag) { SlotCategoryTag = InSlotTag; }

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget> ShortCutBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ShortCutText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPEInventoryHUD> ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ShortCutString;

	FGameplayTag SlotTag;
	FGameplayTag SlotCategoryTag;
	bool IsVaildSlot;

	void SetImageFromTexture(UTexture2D* Texture);
	void SetImageFromSprite(UPaperSprite* Sprite);
	void SetShortCutBoxVisiblity(bool Visibile);
};