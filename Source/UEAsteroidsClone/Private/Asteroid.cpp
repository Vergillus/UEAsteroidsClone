// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

#include "AsteroidDataAsset.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UEAsteroidsClone/UEAsteroidsCloneGameModeBase.h"

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = RootComp;

	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	AsteroidMesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));

}

void AAsteroid::InitializeAsteroid(UAsteroidDataAsset* Data)
{
	DataAsset = Data;

	FVector Direction = (FVector::Zero() - GetActorLocation()).GetSafeNormal();

	constexpr float RandomRotation = 90.0f;
	
	Direction = Direction.RotateAngleAxis(FMath::RandRange(-RandomRotation, RandomRotation), FVector::UpVector);

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), Direction));
	
}

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}

float AAsteroid::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	AUEAsteroidsCloneGameModeBase::SetScore(DataAsset->ReceivedScore);
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}


