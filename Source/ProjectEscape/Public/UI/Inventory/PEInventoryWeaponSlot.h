#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/PEInventoryType.h"
#include "PEInventoryWeaponSlot.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;
class UPEInventoryHUD;

UCLASS()
class PROJECTESCAPE_API UPEInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	void InitEmpty(EInventoryWeaponCategory InCategory);
	virtual void InitSlot(FInventoryRangeWeaponInfo& WeaponInfo);
	virtual void ResetSlot();

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
	EInventoryWeaponCategory Category;

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
class PROJECTESCAPE_API UPEInventoryMeleeWeaponSlot : public UPEInventorySlot
{
	GENERATED_BODY()
};

UCLASS()
class PROJECTESCAPE_API UPEInventoryQuickItemSlot : public UPEInventorySlot
{
	GENERATED_BODY()

public:
	// TODO: 개수 표시
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText;

protected:
	UPROPERTY()
	int32 StackCount;
};

UCLASS()
class PROJECTESCAPE_API UPEInventoryRangeWeaponSlot : public UPEInventorySlot
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	virtual void InitSlot(FInventoryRangeWeaponInfo& WeaponInfo) override;
	virtual void ResetSlot() override;
	void SetSlot(UTexture2D* Texture, int CurrentAmmo, int TotalAmmo);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoCountText;

protected:
	UPROPERTY()
	int32 CurrentAmmoCount;

	UPROPERTY()
	int32 TotalAmmoCount;

	void SetAmmoCount(int Current, int Total);
};
