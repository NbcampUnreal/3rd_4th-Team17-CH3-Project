#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryType.h"
#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "PEInventoryQuickSlot_UsableItem.generated.h"

class UTextBlock;

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickSlotWidget_UsableItem : public UPEInventoryQuickSlot_SlotBase
{
	GENERATED_BODY()

public:
	void SetSlot(FInventoryUsableItemInfo& ItemInfo);
	virtual void ResetSlot() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

private:
	void SetStackCount(int32 Stack, int32 MaxStack, bool Stackable);
};
