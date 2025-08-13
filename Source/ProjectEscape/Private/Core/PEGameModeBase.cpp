#include "Core/PEGameModeBase.h"
#include "Core/PEGameStateBase.h"
#include "Player/PEPlayerController.h"

void APEGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		OnGameStart(GetWorld()->TimeSeconds);
	}
}

void APEGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	Super::EndPlay(EndPlayReason);
}

void APEGameModeBase::GameOver(APlayerController* PlayerController)
{
	if (PlayerController && GetWorld())
	{
		OnGameEnd(GetWorld()->TimeSeconds, false);
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			Pawn->Destroy();
		}
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::GameOverTimerFunction, PlayerController);
		GetWorldTimerManager().SetTimer(GameEndTimer, TimerDelegate, GameEndDuration, false);
	}
}

void APEGameModeBase::GameClear(APlayerController* PlayerController)
{
	if (PlayerController && GetWorld())
	{
		OnGameEnd(GetWorld()->TimeSeconds, true);
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			Pawn->Destroy();
		}
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::GameClearTimerFunction, PlayerController);
		GetWorldTimerManager().SetTimer(GameEndTimer, TimerDelegate, GameEndDuration, false);
	}
}

void APEGameModeBase::OnDamageDealt(float damage)
{
	static const int DamageScoreMultiplier = 2;
	if (FGameResult* GameResult = GetGameResultFromGameState())
	{
		GameResult->DamageDealt += damage;
		GameResult->TotalScore += (int)(damage * DamageScoreMultiplier);
	}
}

void APEGameModeBase::OnKillEnemy(int32 KillCount)
{
	static const int KillScore = 500;
	if (FGameResult* GameResult = GetGameResultFromGameState())
	{
		GameResult->KillCount += KillCount;
		GameResult->TotalScore += KillScore;
	}
}

void APEGameModeBase::OnGameStart(float TimeSeconds)
{
	if (FGameResult* GameResult = GetGameResultFromGameState())
	{
		GameResult->GameStartTime = TimeSeconds;
		GameResult->DamageDealt = 0.0f;
		GameResult->KillCount = 0;
		GameResult->TotalScore = 0;
	}
}

void APEGameModeBase::OnGameEnd(float TimeSeconds, bool IsClear)
{
	static const float TimeBonusBasis = 60.0f * 20; // 20 min
	static const float TimeBonusScoreMultiplier = 50.0f; // 600 sec left -> 30000 score bonus

	if (FGameResult* GameResult = GetGameResultFromGameState())
	{
		GameResult->GameEndTime = TimeSeconds;
		GameResult->GameRunTime = GameResult->GameEndTime - GameResult->GameStartTime;

		float RemainTime = TimeBonusBasis - TimeSeconds;
		if (IsClear)
		{
			GameResult->TotalScore += (RemainTime > 0) ? (RemainTime * TimeBonusScoreMultiplier) : 0;
		}
	}
}

FGameResult* APEGameModeBase::GetGameResultFromGameState()
{
	if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
	{
		return &(PEGameStateBase->GetGameResult());
	}
	return nullptr;
}

void APEGameModeBase::GameOverTimerFunction(APlayerController* PlayerController)
{
	if (PlayerController && GetWorld())
	{
		if (APEPlayerController* PEPlayerController = Cast<APEPlayerController>(PlayerController))
		{
			if (FGameResult* GameResult = GetGameResultFromGameState())
			{
				PEPlayerController->ShowGameOverWidget(*GameResult);
			}
		}
	}
}

void APEGameModeBase::GameClearTimerFunction(APlayerController* PlayerController)
{
	if (PlayerController && GetWorld())
	{
		if (APEPlayerController* PEPlayerController = Cast<APEPlayerController>(PlayerController))
		{
			if (FGameResult* GameResult = GetGameResultFromGameState())
			{
				PEPlayerController->ShowGameClearWidget(*GameResult);
			}
		}
	}
}
