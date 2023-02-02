// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

#include "AsteroidDataAsset.h"
#include "LifespanController.h"
#include "ScreenWarper.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	AsteroidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComponent->ProjectileGravityScale = 0.0f;

	ScreenWarperComp = CreateDefaultSubobject<UScreenWarper>(TEXT("Screen Warper Component"));

	LifespanController = CreateDefaultSubobject<ULifespanController>(TEXT("Lifespawn Controller"));

}

void AAsteroid::InitializeAsteroid(UAsteroidDataAsset* Data)
{
	if(!Data)
	{
		Destroy();
		return;
	}
	
	DataAsset = Data;

	LifespanController->SetLifespan(DataAsset->Lifetime);

	// Get random facing direction
	FVector Direction = (FVector::Zero() - GetActorLocation()).GetSafeNormal();	
	Direction = Direction.RotateAngleAxis(FMath::RandRange(-DataAsset->RandomRotationInDeg, DataAsset->RandomRotationInDeg), FVector::UpVector);

	// Rotate so that forward vector of the object facing the random facing direction
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), Direction));

	// Set movement component velocity
	MovementComponent->Velocity = GetActorForwardVector() * FMath::RandRange(DataAsset->AsteroidMinSpeed, DataAsset->AsteroidMaxSpeed);	
}

float AAsteroid::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if(DataAsset)
	{
		if(const auto GM = Cast<AUEAsteroidsCloneGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// Increase player score
			GM->SetScore(DataAsset->ReceivedScore);

			// If AsteroidType is Large or Medium tell GM to spawn more asteroids
			if (DataAsset->AsteroidType == EAsteroidType::EAT_Large)
			{
				GM->SpawnAsteroidAtPosition(EAsteroidType::EAT_Medium, GetActorLocation());				
			}
			else if (DataAsset->AsteroidType == EAsteroidType::EAT_Medium)
			{
				GM->SpawnAsteroidAtPosition(EAsteroidType::EAT_Small, GetActorLocation(), 2);
			}		
		}
	}

	if(DeadParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,DeadParticle, GetActorLocation());		
	}
	if(ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ExplosionSound,GetActorLocation());
	}
	
	Destroy();	
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}



