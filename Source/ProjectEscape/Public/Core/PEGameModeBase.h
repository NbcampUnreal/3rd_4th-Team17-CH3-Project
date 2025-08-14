#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PEGameModeBase.generated.h"

class APlayerController;
struct FGameResult;

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

	void OnDamageDealt(APlayerController* PlayerController, float Damage);
	void OnKillEnemy(APlayerController* PlayerController, int32 KillScore);
	void OnGameStart(float TimeSeconds);
	void OnGameEnd(APlayerController* PlayerController, float TimeSeconds, bool IsClear);

protected:
	FTimerHandle GameEndTimer;
	float GameEndDuration = 3.0f;

	void GameOverTimerFunction(APlayerController* PlayerController);
	void GameClearTimerFunction(APlayerController* PlayerController);
};
