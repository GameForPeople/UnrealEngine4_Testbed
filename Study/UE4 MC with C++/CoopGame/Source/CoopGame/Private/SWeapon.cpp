// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing ( 
	TEXT("COOP_DebugWeapons"), 
	DebugWeaponDrawing, 
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat
);


// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

// Called when the game starts or when spawned
//void ASWeapon::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

void ASWeapon::Fire()
{
	//Trace For World ; From Pawn Eyes to Crosshair location
	
	AActor* MyOwner = GetOwner();
	
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); // Eye Location, Eye Rotation

		FVector ShortDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShortDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		//Particle "Target" parameter
		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
		{
			// Blocking Hit! Process

			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShortDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
		
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
		}

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		}

		PlayFireEffects(TracerEndPoint);
	}
}

void ASWeapon::PlayFireEffects(FVector TracerEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

	if (TracerEffect)
	{
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter("Target", TracerEndPoint);
		}
	}
}

// Called every frame
//void ASWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

