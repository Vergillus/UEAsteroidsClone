// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

#include "AsteroidDataAsset.h"
#include "ScreenWarper.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UEAsteroidsClone/UEAsteroidsCloneGameModeBase.h"

// Sets default values
AAsteroid::AAsteroid() :
	AsteroidMinSpeed(800.0f),
	AsteroidMaxSpeed(1200.0f),
	RandomRotationInDeg(45),
	Lifetime(5)
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

}

void AAsteroid::InitializeAsteroid(UAsteroidDataAsset* Data)
{
	SetLifeSpan(Lifetime);
	
	DataAsset = Data;

	FVector Direction = (FVector::Zero() - GetActorLocation()).GetSafeNormal();	
	
	Direction = Direction.RotateAngleAxis(FMath::RandRange(-RandomRotationInDeg, RandomRotationInDeg), FVector::UpVector);

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), Direction));

	MovementComponent->Velocity = GetActorForwardVector() * FMath::RandRange(AsteroidMinSpeed, AsteroidMaxSpeed);	
}

float AAsteroid::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{

	if(DataAsset)
	{
		AUEAsteroidsCloneGameModeBase::SetScore(DataAsset->ReceivedScore);

		if (DataAsset->AsteroidType == EAsteroidType::EAT_Large)
		{
			if(const auto GM = Cast<AUEAsteroidsCloneGameModeBase>(UGameplayStatics::GetGameMode(this)))
			{
				GM->SpawnAsteroidAtPosition(EAsteroidType::EAT_Medium, GetActorLocation());
			}
		}
		else if (DataAsset->AsteroidType == EAsteroidType::EAT_Medium)
		{
			if(const auto GM = Cast<AUEAsteroidsCloneGameModeBase>(UGameplayStatics::GetGameMode(this)))
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



