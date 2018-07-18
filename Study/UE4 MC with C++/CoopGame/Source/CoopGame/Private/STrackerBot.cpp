// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"

#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"

#include "GameFramework/Character.h"

#include "DrawDebugHelpers.h"

#include "SHealthComponent.h"
// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);


	bUseVelocityChange = false;
	MovementForce = 1000;
	RequireDistanceToTarget = 100.0f;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent * InputHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	//UE_LOG(LogTemp, Log, TEXT("OMG! I'm Sick! My Health is %s"), *FString::SanitizeFloat(Health), *GetName());
}

FVector ASTrackerBot::GetNextPathPoint()
{
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	UNavigationPath* NavPath = UNavigationSystem::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
	
	if (NavPath->PathPoints.Num() > 1)
	{
		return NavPath->PathPoints[1];
	}

	return GetActorLocation();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//FVector::Distance(GetActorLocation(), NextPathPoint) < RequireDistanceToTarget
	if((GetActorLocation() - NextPathPoint).Size() < RequireDistanceToTarget)
	//if (!GetActorLocation().Equals(NextPathPoint))
	{
		NextPathPoint = GetNextPathPoint();

		//DrawDebugString(GetWorld(), GetActorLocation(), "Test Result is True!");
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();

		ForceDirection *= MovementForce;
		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ForceDirection, 32, FColor::Cyan, false, 0.0f, 1.0f);
		MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
	}

	DrawDebugSphere(GetWorld(), NextPathPoint, 20, 12, FColor::Magenta, false, 0.0f, 1.0f);
}

// Called to bind functionality to input
//void ASTrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

