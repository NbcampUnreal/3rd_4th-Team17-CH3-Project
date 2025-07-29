#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PEInventoryType.h"
#include "PEInventoryHUD.generated.h"

UCLASS()
class PROJECTESCAPE_API UPEInventoryHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPEInventoryHUD(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void InitInventoryUI();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagRowCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagColCount;
};
