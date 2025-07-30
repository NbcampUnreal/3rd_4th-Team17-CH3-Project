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

	void InitEmpty(int32 InSlotIndex);
	void InitSlot(int32 InSlotIndex, FInventoryItemInfo& ItemInfo);

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsValid() const { return IsVaildSlot; };

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly)
	int32 StackCount;

	UPROPERTY(BlueprintReadOnly)
	bool IsStackable;

	UPROPERTY(BlueprintReadOnly)
	bool IsVaildSlot;
};
