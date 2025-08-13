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

	void OnDamageDealt(float damage);
	void OnKillEnemy(int32 KillCount = 1);
	void OnGameStart(float TimeSeconds);
	void OnGameEnd(float TimeSeconds);

protected:
	FGameResult* GetGameResultFromGameState();

	FTimerHandle GameEndTimer;
	float GameEndDuration = 3.0f;

	void GameOverTimerFunction(APlayerController* PlayerController);
	void GameClearTimerFunction(APlayerController* PlayerController);
};
