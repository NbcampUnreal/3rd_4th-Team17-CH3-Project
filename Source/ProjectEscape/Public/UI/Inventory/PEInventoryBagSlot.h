#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryBagSlot.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;
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

	void ResetSlot();
	void SetSlot(UTexture2D* Texture, int Count, bool Stackable);
	void SwapSlot(UPEInventoryBagSlot* Other);
	
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

	UPROPERTY()
	UTexture2D* ItemTexture;

	UPROPERTY()
	int32 StackCount;

	UPROPERTY()
	bool IsStackable;

	UPROPERTY()
	bool IsVaildSlot;

private:
	void SetTexture(UTexture2D* Texture);
	void SetStackCount(int Count, bool Stackable);
};
