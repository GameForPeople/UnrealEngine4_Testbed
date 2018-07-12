// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Section_1_7HUD.generated.h"

UCLASS()
class ASection_1_7HUD : public AHUD
{
	GENERATED_BODY()

public:
	ASection_1_7HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

