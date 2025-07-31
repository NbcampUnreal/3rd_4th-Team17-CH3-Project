#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryType.h"
#include "PEInventoryQuickSlot.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;
class UPEInventoryHUD;

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void InitEmpty(EInventoryItemCategory InCategory);
	virtual void ResetSlot() {}

	void SetParentWidget(UPEInventoryHUD* Parent)
	{
		ParentWidget = Parent;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	bool IsValid() const { return IsVaildSlot; };

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget> ShortCutBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ShortCutText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UPEInventoryHUD> ParentWidget;

	UPROPERTY()
	EInventoryItemCategory Category;

	UPROPERTY()
	UTexture2D* WeaponTexture;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString ShortCutString;

	UPROPERTY()
	bool IsVaildSlot;

	void SetTexture(UTexture2D* Texture);
	void SetShortCutBoxVisiblity(bool Visibile);
};

UCLASS()
class PROJECTESCAPE_API UPEInventoryMeleeWeaponSlot : public UPEInventoryQuickSlot
{
	GENERATED_BODY()

public:
	void InitSlot(FInventoryMeleeWeaponInfo& WeaponInfo);
	virtual void ResetSlot() override;
	void SetSlot(UTexture2D* Texture);
};

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickItemSlot : public UPEInventoryQuickSlot
{
	GENERATED_BODY()

public:
	void InitSlot(FInventoryItemInfo& ItemInfo);
	virtual void ResetSlot() override;
	void SetSlot(UTexture2D* Texture, int32 Stack, int32 MaxStack, bool Stackable);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

protected:
	UPROPERTY()
	int32 StackCount;

	UPROPERTY()
	int32 MaxStackCount;

	UPROPERTY()
	bool IsStackable;

	void SetStackCount(int32 Stack, int32 MaxStack, bool Stackable);
};

UCLASS()
class PROJECTESCAPE_API UPEInventoryRangeWeaponSlot : public UPEInventoryQuickSlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	void InitSlot(FInventoryRangeWeaponInfo& WeaponInfo);
	virtual void ResetSlot() override;
	void SetSlot(UTexture2D* Texture, int32 CurrentAmmo, int32 TotalAmmo);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoCountText;

protected:
	UPROPERTY()
	int32 CurrentAmmoCount;

	UPROPERTY()
	int32 TotalAmmoCount;

	void SetAmmoCount(int32 Current, int32 Total);
};
