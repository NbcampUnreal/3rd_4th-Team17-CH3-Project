#include "UI/Inventory/PEInventoryHUD.h"
#include "UI/Inventory/PEInventoryBagSlot.h"
#include "UI/Inventory/PEInventoryType.h"
#include "UI/Inventory/PEInventoryWeaponSlot.h"
#include "Components/UniformGridPanel.h"


UPEInventoryHUD::UPEInventoryHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BagRowCount = 2;
	BagColCount = 5;
}

void UPEInventoryHUD::NativeOnInitialized()
{
}

void UPEInventoryHUD::InitInventoryUI(FInventoryInfo InInventoryInfo)
{
	InventoryInfo = InInventoryInfo;

	if (BagSlots.IsEmpty())
	{
		CreateEmptyBagSlots();
	}
	else
	{
		ResetBagSlots();
	}
	InitBagSlots();

	if (MainWeaponSlot)
	{
		MainWeaponSlot->InitEmpty(EInventoryWeaponCategory::MAIN_WEAPON);
	}
	if (SubWeaponSlot)
	{
		SubWeaponSlot->InitEmpty(EInventoryWeaponCategory::SUB_WEAPON);
	}
	if (MeleeWeaponSlot)
	{
		MeleeWeaponSlot->InitEmpty(EInventoryWeaponCategory::MELEE_WEAPON);
	}
	// TODO: 여기서부터
	//if (HealItemSlot)
	//{
	//	HealItemSlot->InitEmpty(EInventoryWeaponCategory:);
	//}
	//if (GrenadeSlot)
	//{
	//	GrenadeSlot->InitEmpty(EInventoryWeaponCategory::MAIN_WEAPON);
	//}
}

void UPEInventoryHUD::DropItemFromBagToLand(int32 Index)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] DropItemFromBagToLand Index: %d"), Index);

	// Remove From UI
	ResetSlot(Index);

	// Notify to Character Component
	// To Be Added.
}

void UPEInventoryHUD::SwapItemInBag(int32 Index, int32 OhterIndex)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] SwapItemInBag Index: %d, OhterIndex: %d"), Index, OhterIndex);

	// Swap In UI
	SwapSlot(Index, OhterIndex);

	// Notify to Character Component
	// To Be Added.
}


void UPEInventoryHUD::CreateEmptyBagSlots()
{
	if (BagSlotUniformGrid && BagSlotClass)
	{
		for (int Row = 0; Row < BagRowCount; Row++)
		{
			for (int Col = 0; Col < BagColCount; Col++)
			{
				UPEInventoryBagSlot* NewSlot = CreateWidget<UPEInventoryBagSlot>(this, BagSlotClass);
				int32 Index = Row * BagColCount + Col;
				NewSlot->SetParentWidget(this);
				NewSlot->InitEmpty(Index);

				BagSlotUniformGrid->AddChildToUniformGrid(NewSlot, Row, Col);
				BagSlots.Add(Index, NewSlot);
			}
		}
	}
}

void UPEInventoryHUD::ResetBagSlots()
{
	for (int Row = 0; Row < BagRowCount; Row++)
	{
		for (int Col = 0; Col < BagColCount; Col++)
		{
			int32 Index = Row * BagColCount + Col;
			ResetSlot(Index);
		}
	}
}

void UPEInventoryHUD::InitBagSlots()
{
	if (BagSlotUniformGrid)
	{
		for (auto& BagInfo : InventoryInfo.Bags)
		{
			int32 Index = BagInfo.Key;
			FInventoryItemInfo& ItemInfo = BagInfo.Value;

			if (BagSlots.Contains(Index))
			{
				if (UPEInventoryBagSlot* BagSlot = Cast<UPEInventoryBagSlot>(BagSlots[Index]))
				{
					BagSlot->InitSlot(Index, ItemInfo);
				}
			}
		}
	}
}

void UPEInventoryHUD::ResetSlot(int32 Index)
{
	if (BagSlots.Contains(Index))
	{
		if (UPEInventoryBagSlot* BagSlot = Cast<UPEInventoryBagSlot>(BagSlots[Index]))
		{
			BagSlot->ResetSlot();
		}
	}
}

void UPEInventoryHUD::SwapSlot(int32 Index, int32 OtherIndex)
{
	if (BagSlots.Contains(Index) && BagSlots.Contains(OtherIndex))
	{
		UPEInventoryBagSlot* BagSlot1 = Cast<UPEInventoryBagSlot>(BagSlots[Index]);
		UPEInventoryBagSlot* BagSlot2 = Cast<UPEInventoryBagSlot>(BagSlots[OtherIndex]);
		if (BagSlot1 && BagSlot2)
		{
			BagSlot1->SwapSlot(BagSlot2);
		}
	}
}
