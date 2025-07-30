#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryBagSlot.generated.h"

class UTextBlock;
class UImage;
class UPEInventoryHUD;
struct FInventoryItemInfo;

UCLASS()
class PROJECTESCAPE_API UPEInventoryBagSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void InitEmpty(int32 InSlotIndex);
	void InitSlot(int32 InSlotIndex, FInventoryItemInfo& ItemInfo);
	
	void SetParentWidget(UPEInventoryHUD* Parent)
	{
		ParentWidget = Parent;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsValid() const { return IsVaildSlot; };

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPEInventoryHUD> ParentWidget;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly)
	int32 StackCount;

	UPROPERTY(BlueprintReadOnly)
	bool IsStackable;

	UPROPERTY(BlueprintReadOnly)
	bool IsVaildSlot;
};
