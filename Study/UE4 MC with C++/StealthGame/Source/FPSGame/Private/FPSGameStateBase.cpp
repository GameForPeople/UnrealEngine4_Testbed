// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameStateBase.h"

#include "Engine/World.h"


void AFPSGameStateBase::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess) 
{
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		// Please Think, Your Game have Many Pawn! OMG! OMG!
		APawn* Pawn = It->Get();
		if (Pawn && Pawn->IsLocallyControlled())
		{
			Pawn->DisableInput(nullptr);
		}
	}
}



