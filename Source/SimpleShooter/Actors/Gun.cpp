// Fill out your copyright notice in the Description page of Project Settings.

#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// #include <activation.h>

#define OUT

AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AGun::PullTrigger()
{
	// Spawn fire particle and sound
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));

	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(OUT Hit, ShotDirection);

	if (bSuccess)
	{
		// Spawn impact particle and sound
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if (!HitActor) { return; }

		// Cause damage to enemy
		FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
		AController* OwnerController = GetOwnerController();
		HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
	}
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) { return false; }

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(OUT Location, OUT Rotation); // Get the loc & rot of pawn's camera
	ShotDirection = -Rotation.Vector();

	// Rotation.Vector() gives you the vector pointing in the direction of the provided rotation
	const FVector End = Location + Rotation.Vector() * MaxRange;

	// Create a list of actors for this line trace to ignore
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(OUT Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1,
		Params);
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) { return nullptr; }
	return OwnerPawn->GetController();
}
