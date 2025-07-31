#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "UI/Inventory/PEInventoryType.h"
#include "PEInventoryBagSlot_Widget.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;
class UPEInventoryHUD;
class UPEInventoryBagSlot_Element;

UCLASS()
class PROJECTESCAPE_API UPEInventoryBagSlot_Widget : public UUserWidget
{
	GENERATED_BODY()

public:
	void CreateEmptyBagSlots();

	void SetParentWidget(UPEInventoryHUD* Parent)
	{
		ParentWidget = Parent;
	}

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UniformGridPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UPEInventoryBagSlot_Element> BagSlotClass;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPEInventoryHUD> ParentWidget;

	TMap<int32, UPEInventoryBagSlot_Element*> BagSlots;
	const int32 BagRowCount = 2;
	const int32 BagColCount = 5;

	void ResetBagSlots();
	void InitBagSlots();

	void ResetSlot(int32 Index);
	void SwapSlot(int32 Index, int32 OtherIndex);
};
