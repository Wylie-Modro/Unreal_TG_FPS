// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Unreal_TG_FPSGameMode.h"
#include "Unreal_TG_FPSHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnreal_TG_FPSGameMode::AUnreal_TG_FPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/Behaviour/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnreal_TG_FPSHUD::StaticClass();
}
