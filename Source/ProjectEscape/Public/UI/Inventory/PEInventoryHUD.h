#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryType.h"
#include "PEInventoryHUD.generated.h"

class UUniformGridPanel;
class UUserWidget;

UCLASS()
class PROJECTESCAPE_API UPEInventoryHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPEInventoryHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void InitInventoryUI(FInventoryInfo InInventoryInfo);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> BagSlotUniformGrid;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> BagSlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagRowCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagColCount;

	FInventoryInfo InventoryInfo;


private:
	void InitEmptyBagSlot();
	void InitBagSlot();

	TMap<int32, UUserWidget*> BagSlots;
};
