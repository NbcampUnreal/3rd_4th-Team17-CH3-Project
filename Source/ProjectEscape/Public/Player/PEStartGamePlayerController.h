#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PEStartGamePlayerController.generated.h"

UCLASS()
class PROJECTESCAPE_API APEStartGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void OnClickCredit();

	UFUNCTION(BlueprintCallable)
	void OnClickHelper();

	UFUNCTION(BlueprintCallable)
	void OnClickBackMainMenu();

	void ClearAllWidget();
	void ShowMainMenu();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> CreditWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI|Class")
	TSubclassOf<UUserWidget> HelperWidgetClass;

	UUserWidget* MainMenuWidget;
	UUserWidget* CreditWidget;
	UUserWidget* HelperWidget;
};
