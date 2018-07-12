// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Section_1_7GameMode.h"
#include "Section_1_7HUD.h"
#include "Section_1_7Character.h"
#include "UObject/ConstructorHelpers.h"

ASection_1_7GameMode::ASection_1_7GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASection_1_7HUD::StaticClass();
}
