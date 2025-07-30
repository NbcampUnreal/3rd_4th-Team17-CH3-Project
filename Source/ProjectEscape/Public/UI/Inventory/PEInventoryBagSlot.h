#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryBagSlot.generated.h"

class UTextBlock;
class UImage;
struct FInventoryItemInfo;

UCLASS()
class PROJECTESCAPE_API UPEInventoryBagSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void InitEmpty(uint32 InSlotIndex);
	void InitSlot(uint32 InSlotIndex, FInventoryItemInfo& ItemInfo);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsValid() const { return IsVaildSlot; };

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

protected:
	uint32 SlotIndex;
	uint32 StackCount;
	bool IsStackable;
	bool IsVaildSlot;
};
