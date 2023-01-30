// Fill out your copyright notice in the Description page of Project Settings.


#include "UEAsteroidsClone/Public/Spaceship.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ScreenWarper.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UEAsteroidsClone/UEAsteroidsCloneGameModeBase.h"

#include "UEAsteroidsClone/Public/Bullet.h"

// Sets default values
ASpaceship::ASpaceship() :
	RotationSpeed(100.0f),
	BulletSpawnSocketName("BulletSpawnLoc")
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	RootComponent = RootComp;
	RootComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceship::OnOverlapBegin);

	SpaceshipMesh = CreateDefaultSubobject<UStaticMeshComponent>("Spaceship Mesh");
	SpaceshipMesh->SetupAttachment(RootComponent);
	SpaceshipMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PawnMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>("Pawn Movement Component");

	ScreenWarperComp = CreateDefaultSubobject<UScreenWarper>(TEXT("Screen Warper"));

}

// Called when the game starts or when spawned
void ASpaceship::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SpaceshipMappingContext,0);
		}		
	}	
}

// Called to bind functionality to input
void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpaceship::Move);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ASpaceship::RotateShip);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASpaceship::Fire);
		
	}

}

void ASpaceship::Move(const FInputActionValue& Value)
{
	PawnMovementComp->AddInputVector(GetActorForwardVector());
}

void ASpaceship::RotateShip(const FInputActionValue& Value)
{
	const float InputVal = Value.Get<float>();

	AddActorWorldRotation(FRotator{0,InputVal * RotationSpeed * GetWorld()->DeltaTimeSeconds,0});
	
}

void ASpaceship::Fire(const FInputActionValue& Value)
{
	if(!BulletToSpawn) return;

	const FTransform BulletSpawnTransform = SpaceshipMesh->GetSocketTransform(BulletSpawnSocketName);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;

	GetWorld()->SpawnActor<ABullet>(BulletToSpawn,BulletSpawnTransform.GetLocation(), BulletSpawnTransform.Rotator(),SpawnParameters);

	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound,BulletSpawnTransform.GetLocation());
	}
}

void ASpaceship::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		UE_LOG(LogTemp,Warning, TEXT("DEAD Collision"));

		if(!OtherActor->IsPendingKillPending())
		{
			OtherActor->Destroy();			
		}

		OnDeathHandler();
	}
}

void ASpaceship::OnDeathHandler()
{
	DisableInput(nullptr);
	RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);

	if(ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound,GetActorLocation());
	}
	if(ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticle, GetActorLocation());
	}		

	if(const auto GM = Cast<AUEAsteroidsCloneGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->PlayerDead();
	}
}

void ASpaceship::OnRetryHandler()
{
	SetActorHiddenInGame(false);
	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
	RootComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

