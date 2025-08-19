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
		OnGameEnd(PlayerController, GetWorld()->TimeSeconds, false);
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
		OnGameEnd(PlayerController, GetWorld()->TimeSeconds, true);
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			Pawn->Destroy();
		}
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &ThisClass::GameClearTimerFunction, PlayerController);
		GetWorldTimerManager().SetTimer(GameEndTimer, TimerDelegate, GameEndDuration, false);
	}
}

void APEGameModeBase::OnDamageDealt(APlayerController* PlayerController, float Damage)
{
	static const int DamageScoreMultiplier = 2;
	if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
	{
		PEGameStateBase->AddDamageDealt(Damage);

		int32 DamageScore = (int32)(Damage * DamageScoreMultiplier);
		PEGameStateBase->AddTotalScore(PlayerController, DamageScore);
	}
}

void APEGameModeBase::OnKillEnemy(APlayerController* PlayerController, int32 KillScore)
{
	if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
	{
		PEGameStateBase->AddKillScore(KillScore);
	}
}

void APEGameModeBase::OnGameStart(float TimeSeconds)
{
	if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
	{
		PEGameStateBase->Reset();
		PEGameStateBase->SetGameStart(TimeSeconds);
	}
}

void APEGameModeBase::OnGameEnd(APlayerController* PlayerController, float TimeSeconds, bool IsClear)
{
	static const float TimeBonusBasis = 60.0f * 5; // 5 min
	static const float TimeBonusScoreMultiplier = 50.0f; // 600 sec left -> 30000 score bonus
	if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
	{
		PEGameStateBase->SetGameEnd(TimeSeconds);
		float RemainTime = TimeBonusBasis - TimeSeconds;
		if (IsClear)
		{
			int32 TimeBonusScore = (RemainTime > 0) ? (RemainTime * TimeBonusScoreMultiplier) : 0;
			PEGameStateBase->AddTotalScore(PlayerController, TimeBonusScore);
		}
	}
}

void APEGameModeBase::GameOverTimerFunction(APlayerController* PlayerController)
{
	if (PlayerController && GetWorld())
	{
		if (APEPlayerController* PEPlayerController = Cast<APEPlayerController>(PlayerController))
		{
			if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
			{
				PEPlayerController->ShowGameOverWidget(PEGameStateBase->GetGameResult());
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
			if (APEGameStateBase* PEGameStateBase = GetGameState<APEGameStateBase>())
			{
				PEPlayerController->ShowGameClearWidget(PEGameStateBase->GetGameResult());
			}
		}
	}
}
