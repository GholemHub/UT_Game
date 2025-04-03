// Copyright Epic Games, Inc. All Rights Reserved.

#include "UT_GameGameMode.h"
#include "UT_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUT_GameGameMode::AUT_GameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
