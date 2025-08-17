#include "UI/Inventory/PEInventoryHUD.h"

#include "Characters/Hero/PEHero.h"
#include "UI/Inventory/PEInventoryBagSlot_Widget.h"
#include "UI/Inventory/PEInventoryQuickSlot_Widget.h"
#include "UI/Inventory/PEInventoryType.h"


void UPEInventoryHUD::NativeOnInitialized()
{
	if (BagSlotWidget)
	{
		BagSlotWidget->CustomOnInitialized(this);
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->CustomOnInitialized(this);
	}
}

void UPEInventoryHUD::SetupComponentReference(UActorComponent* InIntenvoryBagComponent, UActorComponent* InEquipmentQuickSlotComponent, AActor* InHero)
{
	// Interface, To be confirmed.
	IntenvoryBagComponent = InIntenvoryBagComponent;
	EquipmentQuickSlotComponent = InEquipmentQuickSlotComponent;
	Hero = InHero;
}

void UPEInventoryHUD::UpdateInventoryUI(UPARAM(ref) FInventoryInfo& InInventoryInfo)
{
	if (BagSlotWidget)
	{
		BagSlotWidget->UpdateInventoryUI(InInventoryInfo);
	}

	if (QuickSlotWidget)
	{
		QuickSlotWidget->UpdateInventoryUI(InInventoryInfo);
	}
}

void UPEInventoryHUD::DropItemFromBagToLand(const FGameplayTag& Tag, const int32& Count)
{
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] DropItemFromBagToLand Tag: %s"), *Tag.ToString());

	if (APEHero* HeroActor = Cast<APEHero>(Hero))
	{
		HeroActor->OnInventoryItemDrop.Broadcast(Tag, Count);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[DEBUG] Failed to cast Hero to APEHero"));
	}
}

void UPEInventoryHUD::UnequipWeaponAndDropToLand(FGameplayTag WeaponTag)
{
	FString TagString = WeaponTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] UnequipWeaponAndDropToLand Tag: %s"), *TagString);

	if (APEHero* HeroActor = Cast<APEHero>(Hero))
	{
		HeroActor->OnQuickSlotEquipmentDrop.Broadcast(WeaponTag);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[DEBUG] Failed to cast Hero to APEHero"));
	}
}

void UPEInventoryHUD::RemoveItemFromQucikSlotAndDropToLand(FGameplayTag ItemTag)
{
	FString TagString = ItemTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] RemoveItemFromQucikSlotAndDropToLand Tag: %s"), *TagString);

	
	if (APEHero* HeroActor = Cast<APEHero>(Hero))
	{
		HeroActor->OnQuickSlotEquipmentDrop.Broadcast(ItemTag);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[DEBUG] Failed to cast Hero to APEHero"));
	}
}

void UPEInventoryHUD::AddItemFromBagToQuickSlot(FGameplayTag ItemTag)
{
	FString TagString = ItemTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] AddItemFromBagToQuickSlot Tag: %s"), *TagString);
	
	if (APEHero* HeroActor = Cast<APEHero>(Hero))
	{
		HeroActor->UseItemByInventory(ItemTag);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[DEBUG] Failed to cast Hero to APEHero"));
	}
}

void UPEInventoryHUD::RemoveItemFromQuickSlotToBag(FGameplayTag ItemTag)
{
	FString TagString = ItemTag.ToString();
	UE_LOG(LogTemp, Display, TEXT("[DEBUG] RemoveItemFromQuickSlotToBag Tag: %s"), *TagString);

	// Notify to Character Component
	// To Be Added.
}
