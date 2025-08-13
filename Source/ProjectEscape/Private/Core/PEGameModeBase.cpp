#include "Core/PEGameModeBase.h"

void APEGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APEGameModeBase::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void APEGameModeBase::GameOver(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			Pawn->Destroy();
		}
	}
	
}

void APEGameModeBase::GameClear(APlayerController* PlayerController)
{
}
