#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PEGameModeBase.generated.h"

class APlayerController;

UCLASS()
class PROJECTESCAPE_API APEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable)
	void GameOver(APlayerController* PlayerController);
	
	UFUNCTION(BlueprintCallable)
	void GameClear(APlayerController* PlayerController);
};
