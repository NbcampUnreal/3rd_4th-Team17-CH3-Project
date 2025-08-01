#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "UI/Inventory/PEInventoryType.h"
#include "PEInventoryBagSlot_Element.generated.h"

class UTextBlock;
class UImage;
class UPaperSprite;
class UTexture2D;
class UPEInventoryHUD;

UCLASS()
class PROJECTESCAPE_API UPEInventoryBagSlot_Element : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlot(FInventoryBagSlotInfo& ItemInfo);
	void ResetSlot();

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
	int32 GetSlotIndex() const { return SlotIndex; }

	UFUNCTION(BlueprintCallable)
	void SetSlotIndex(int32 InSlotIndex) { SlotIndex = InSlotIndex; }

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPEInventoryHUD> ParentWidget;

	FGameplayTag SlotTag;
	int32 SlotIndex;
	bool IsVaildSlot;

private:
	void SetImageFromTexture(UTexture2D* Texture);
	void SetImageFromSprite(UPaperSprite* Sprite);
	void SetStackCount(int Count, int MaxCount, bool Stackable);
};
