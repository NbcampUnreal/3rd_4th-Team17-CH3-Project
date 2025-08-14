#include "Core/PEGameStateBase.h"

void APEGameStateBase::Reset()
{
	GameResult.DamageDealt = 0.0f;
	GameResult.KillCount = 0;
	GameResult.TotalScore = 0;
}

void APEGameStateBase::AddDamageDealt(float Damage)
{
	GameResult.DamageDealt += Damage;
}

void APEGameStateBase::AddKillScore(int32 KillScore)
{
	GameResult.KillCount += 1;
	GameResult.TotalScore += KillScore;
}

void APEGameStateBase::SetGameStart(float TimeSeconds)
{
	GameResult.GameStartTime = TimeSeconds;
}

void APEGameStateBase::SetGameEnd(float TimeSeconds)
{
	GameResult.GameEndTime = TimeSeconds;
	GameResult.GameRunTime = GameResult.GameEndTime - GameResult.GameStartTime;
}

void APEGameStateBase::AddTotalScore(int32 TotalScore)
{
	GameResult.TotalScore += TotalScore;
}
