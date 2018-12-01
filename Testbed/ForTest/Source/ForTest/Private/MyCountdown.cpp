// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCountdown.h"

#include "Components/TextRenderComponent.h"
#include "TimerManager.h"

// Sets default values
AMyCountdown::AMyCountdown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	countdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("countdownNumber"));
	countdownText->SetHorizontalAlignment(EHTA_Center);
	countdownText->SetWorldSize(150.0f);

	RootComponent = countdownText;

	countdownTime = 3;
}

// Called when the game starts or when spawned
void AMyCountdown::BeginPlay()
{
	Super::BeginPlay();

	UpdateTimerDisplay();
	GetWorldTimerManager().SetTimer(countdownTimerHandle, this, &AMyCountdown::AdvanceTimer, 1.0f, true);
}

// Called every frame
//void AMyCountdown::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

void AMyCountdown::UpdateTimerDisplay()
{
	countdownText->SetText(FString::FromInt(FMath::Max(countdownTime, 0)));
}

void AMyCountdown::AdvanceTimer()
{
	--countdownTime;
	UpdateTimerDisplay();

	if (countdownTime < 1)
	{
		GetWorldTimerManager().ClearTimer(countdownTimerHandle);
		CountdownHasFinished();
	}
}

void AMyCountdown::CountdownHasFinished_Implementation()
{
	countdownText->SetText(TEXT("Go!"));
}

