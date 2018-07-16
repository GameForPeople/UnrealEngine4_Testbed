// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarret.h"

#include "SHealthComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "Particles/ParticleSystem.h"
#include "Engine/World.h"


// Sets default values
ASExplosiveBarret::ASExplosiveBarret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	//HealthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarret::OnHealthChanged);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);

	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 250;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;

	ExplosionImpulse = 400;
}

//void ASExplosiveBarret::OnHealthChanged(USHealthComponent * HealthComp, float Health, float HealthDelta,
//	const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
//{
//	if (bExploded)
//	{
//		return;
//	}
//
//	if (Health <= 0.0f)
//	{
//		bExploded = true;
//
//		FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
//		MeshComp->AddImpulse(BoostIntensity, NAME_None, true);
//
//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
//		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, (FVector)this->GetActorLocation());
//
//		MeshComp->SetMaterial(0, ExplodedMaterial);
//
//		RadialForceComp->FireImpulse();
//	}
//}





// Called when the game starts or when spawned
//void ASExplosiveBarret::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ASExplosiveBarret::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

