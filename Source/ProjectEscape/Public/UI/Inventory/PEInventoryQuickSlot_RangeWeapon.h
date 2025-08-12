#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryType.h"
#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "PEInventoryQuickSlot_RangeWeapon.generated.h"

class UTextBlock;

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickSlotWidget_RangeWeapon : public UPEInventoryQuickSlot_SlotBase
{
	GENERATED_BODY()

public:
	void SetSlot(FInventoryRangeWeaponInfo& WeaponInfo);
	virtual void ResetSlot() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoCountText;

protected:
	void SetAmmoCount(int32 Current, int32 Total);
};
