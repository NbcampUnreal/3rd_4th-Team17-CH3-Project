#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryType.h"
#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "PEInventoryQuickSlot_MeleeWeapon.generated.h"

class UTextBlock;

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickSlotWidget_MeleeWeapon : public UPEInventoryQuickSlot_SlotBase
{
	GENERATED_BODY()

public:
	void SetSlot(FInventoryMeleeWeaponInfo& WeaponInfo);
	virtual void ResetSlot() override;
};
