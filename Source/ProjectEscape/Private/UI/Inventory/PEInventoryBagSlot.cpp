#include "UI/Inventory/PEInventoryBagSlot.h"


// temporal class
class IInventoryBagItem
{

};

void UPEInventoryBagSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPEInventoryBagSlot::InitSlot(uint32 InSlotIndex, IInventoryBagItem& BagItem)
{
	SlotIndex = InSlotIndex;
}
