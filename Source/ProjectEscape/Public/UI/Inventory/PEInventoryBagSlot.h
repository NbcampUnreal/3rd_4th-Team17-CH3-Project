#pragma once
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryBagSlot.generated.h"

class UTextBlock;
class UImage;
class IInventoryBagItem;

UCLASS()
class PROJECTESCAPE_API UPEInventoryBagSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void InitSlot(uint32 InSlotIndex, IInventoryBagItem& BagItem);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;


protected:
	uint32 SlotIndex;
	uint32 StackCount;
	bool IsStackable;
};
