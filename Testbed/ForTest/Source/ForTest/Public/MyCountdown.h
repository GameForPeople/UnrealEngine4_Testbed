// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyCountdown.generated.h"

class UTextRenderComponent;

UCLASS()
class FORTEST_API AMyCountdown : public AActor
{
	GENERATED_BODY()
	
protected:
	//초 단위의 카운트다운 실행 시간입니다.
	UPROPERTY(EditAnywhere)
	int32 countdownTime;

	UTextRenderComponent* countdownText;

	FTimerHandle countdownTimerHandle;

public:	
	// Sets default values for this actor's properties
	AMyCountdown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	
	void UpdateTimerDisplay();
	void AdvanceTimer();

	UFUNCTION(BlueprintNativeEvent)
	void CountdownHasFinished();
	virtual void CountdownHasFinished_Implementation();
};
