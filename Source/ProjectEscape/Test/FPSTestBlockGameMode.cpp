// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSTestBlockGameMode.h"
#include "FPSTestBlockCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSTestBlockGameMode::AFPSTestBlockGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
