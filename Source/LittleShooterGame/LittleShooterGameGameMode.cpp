// Copyright Epic Games, Inc. All Rights Reserved.

#include "LittleShooterGameGameMode.h"
#include "LittleShooterGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALittleShooterGameGameMode::ALittleShooterGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
