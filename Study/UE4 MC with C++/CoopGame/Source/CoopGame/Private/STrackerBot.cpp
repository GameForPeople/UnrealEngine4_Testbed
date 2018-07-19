// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"

#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"

#include "GameFramework/Character.h"

#include "DrawDebugHelpers.h"

#include "SHealthComponent.h"

#include "SCharacter.h"

#include "Sound/SoundCue.h"


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

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereComp->SetupAttachment(RootComponent);

	bUseVelocityChange = false;
	MovementForce = 800;
	RequireDistanceToTarget = 300.0f;

	ExplosionDamage = 40;
	ExplosionRadius = 200;

	SelfDamageInterval = 0.25f;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	NextPathPoint = GetNextPathPoint();

	//OnActorBeginOverlap.AddDynamic();
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent * InputHealthComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	//UE_LOG(LogTemp, Log, TEXT("OMG! I'm Sick! My Health is %s"), *FString::SanitizeFloat(Health), *GetName());

	if (MatInst == nullptr)
	{
		MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}

	if (MatInst) 
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}
	
	if (Health <= 0) 
	{
		SelfDestruct();
	}
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

void ASTrackerBot::SelfDestruct()
{
	if (bExploded) 
	{
		return;
	}

	bExploded = true;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoreActors, this, GetInstigatorController(), true);

	UGameplayStatics::SpawnSoundAttached(ExplodeSound, RootComponent); // , NAME_None, GetActorLocation());

	DrawDebugSphere(GetWorld(), NextPathPoint, ExplosionRadius, 12, FColor::Red, false, 2.0, 0.0f, 1.0f);

	Destroy();
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
		
		//MovementForce = ForceDirection.Size();
		//ForceDirection.Normalize();
		//ForceDirection *= (MovementForce * 2);
		
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

void ASTrackerBot::NotifyActorBeginOverlap(AActor * OtherActor)
{
	if (!bStartedSelfDestruction) {

		ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);

		if (PlayerPawn)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.0f);

			bStartedSelfDestruction = true;

			//UGameplayStatics::PlaySoundAttached(SelfDestructSound, RootComponent);
			UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent); // , NAME_None, GetActorLocation());

		}
	}
}

void ASTrackerBot::DamageSelf()
{
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}
