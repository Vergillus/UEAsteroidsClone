// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUFO.h"

#include "Bullet.h"
#include "LifespanController.h"
#include "ScreenWarper.h"
#include "UFODataAsset.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UEAsteroidsClone/UEAsteroidsCloneGameModeBase.h"

// Sets default values
AEnemyUFO::AEnemyUFO()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("Root Component"));
	RootComponent = RootComp;

	UFOMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UFO Mesh"));
	UFOMesh->SetupAttachment(RootComponent);
	UFOMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	MovementComponent->ProjectileGravityScale = 0.0f;

	ScreenWarperComp = CreateDefaultSubobject<UScreenWarper>(TEXT("Screen Warper Component"));

	LifespanController = CreateDefaultSubobject<ULifespanController>(TEXT("Lifespan Controller"));

	UFOAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));	

}

void AEnemyUFO::InitializeUFO(UUFODataAsset* DataAsset)
{
	if(!DataAsset)
	{
		Destroy();
		return;
	}
	
	UFOData = DataAsset;

	LifespanController->SetLifespan(UFOData->LifeSpan);	

	// Get random facing direction
	FVector Direction = (FVector::Zero() - GetActorLocation()).GetSafeNormal();	
	Direction = Direction.RotateAngleAxis(FMath::RandRange(-RandomRotationInDeg, RandomRotationInDeg), FVector::UpVector);

	// Rotate so that forward vector of the object facing the random facing direction
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorForwardVector(), Direction));
	
	MovementComponent->Velocity = GetActorForwardVector() * FMath::RandRange(UFOData->UFOMinSpeed, UFOData->UFOMaxSpeed);

	// Start Cooldown Timer with delay
	GetWorldTimerManager().SetTimer(AttackCooldownTimerHandle, this, &AEnemyUFO::CooldownTimerElapsed,
	                                UFOData->AttackCooldownDuration, true, UFOData->AttackCooldownDuration);
}


void AEnemyUFO::CooldownTimerElapsed()
{
	Attack();
}

void AEnemyUFO::Attack()
{
	if (const APawn* PlayerPawn =  UGameplayStatics::GetPlayerPawn(this,0))
	{
		const FVector BulletFacingDirection = (PlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if(ABullet* BulletClone = GetWorld()->SpawnActor<ABullet>(UFOData->BulletToSpawn, GetActorLocation(), BulletFacingDirection.Rotation(),SpawnParameters))
		{
			// Set bullets to root collider to ignore collision with Asteroid 
			BulletClone->GetRootCollider()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Ignore);
			BulletClone->Tags.Add(EnemyBulletTagName);
			BulletClone->InitializeBullet(BulletFacingDirection * UFOData->BulletSpeed);
		}
	}
}

float AEnemyUFO::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (UFOData)
	{
		if(const auto GM = Cast<AUEAsteroidsCloneGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GM->SetScore(UFOData->ReceivedScore);
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



