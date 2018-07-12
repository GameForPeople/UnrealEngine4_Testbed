// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameStateBase.h"

#include "FPSPlayerController.h"
#include "Engine/World.h"


void AFPSGameStateBase::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess) 
{
//	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
//	{
//		// Please Think, Your Game have Many Pawn! OMG! OMG!
//		APawn* Pawn = It->Get();
//		if (Pawn && Pawn->IsLocallyControlled())
//		{
//			Pawn->DisableInput(nullptr);
//		}
//	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(It->Get());
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(InstigatorPawn, bMissionSuccess);

			APawn* MyPawn = PC->GetPawn();

			if (MyPawn)
			{
				MyPawn->DisableInput(PC);
			}
		}
	}
}



