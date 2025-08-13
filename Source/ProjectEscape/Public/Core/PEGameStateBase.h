#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PEGameStateBase.generated.h"

struct FGameResult
{
	int32 KillCount = 0;
	float GameStartTime = 0.0f;
	float GameEndTime = 0.0f;
	float GameRunTime = 0.0f;
	float DamageDealt = 0.0f;
	int32 TotalScore = 0;
};

UCLASS()
class PROJECTESCAPE_API APEGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	FGameResult& GetGameResult()
	{
		return GameResult;
	}

protected:
	FGameResult GameResult;
};
