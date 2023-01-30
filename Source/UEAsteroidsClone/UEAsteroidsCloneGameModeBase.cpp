// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEAsteroidsCloneGameModeBase.h"

#include "Spaceship.h"
#include "Kismet/GameplayStatics.h"

int32 AUEAsteroidsCloneGameModeBase::Score = 0;

AUEAsteroidsCloneGameModeBase::AUEAsteroidsCloneGameModeBase() :
	PlayerCurrentRetryAmount(0),
	AsteroidSpawnInterval(1),
	UFOSpawnInterval(1),
	PlayerRetryDelay(1)
{
}


void AUEAsteroidsCloneGameModeBase::BeginPlay()
{
	Score = 0;
	Super::BeginPlay();

	constexpr  float InFirstDelay = 1.0f;
	GetWorldTimerManager().SetTimer(AsteroidSpawnTimerHandle,this,&AUEAsteroidsCloneGameModeBase::SpawnAsteroidTimerElapsed,AsteroidSpawnInterval, true, InFirstDelay);
	
}

void AUEAsteroidsCloneGameModeBase::SpawnAsteroid(UAsteroidDataAsset* AsteroidData) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetRandomSpawnPoint());
	SpawnTransform.SetScale3D(FVector::One() * AsteroidData->UniformScale);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	if(const auto AsteroidClone = GetWorld()->SpawnActor<AAsteroid>(AsteroidToSpawn,SpawnTransform,SpawnParameters))
	{
		AsteroidClone->InitializeAsteroid(AsteroidData);
	}
}

void AUEAsteroidsCloneGameModeBase::SpawnAsteroidAtPosition(UAsteroidDataAsset* AsteroidData, FVector SpawnPosition,
	int Amount)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpawnTransform;	
	SpawnTransform.SetScale3D(FVector::One() * AsteroidData->UniformScale);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	const float RandomPointsRadius = 10.0f; 

	for (int i = 0; i < Amount; ++i)
	{
		// Get a random point in circle
		const FVector2D RandomPointInCircle = FMath::RandPointInCircle(RandomPointsRadius);
		SpawnTransform.SetLocation(SpawnPosition + FVector(RandomPointInCircle.X, RandomPointInCircle.Y, 0));
		
		if(const auto AsteroidClone = GetWorld()->SpawnActor<AAsteroid>(AsteroidToSpawn,SpawnTransform,SpawnParameters))
		{
			AsteroidClone->InitializeAsteroid(AsteroidData);
		}		
	}
}

void AUEAsteroidsCloneGameModeBase::SpawnAsteroidTimerElapsed()
{
	SpawnAsteroid(AsteroidsDataArray[FMath::RandRange(0, AsteroidsDataArray.Num() - 1)]);
}

FVector AUEAsteroidsCloneGameModeBase::GetRandomSpawnPoint() const
{
	FVector RndWorldSpawnLoc;

	int32 ViewportSizeX;
	int32 ViewportSizeY;

	const ESpawnDirections RndSpawnPositionDirection = SpawnDirections[FMath::RandRange(0, SpawnDirections.Num() - 1)];

	UGameplayStatics::GetPlayerController(this, 0)->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float RndScreenLocX = 0; 
	float RndScreenLocY = 0; 

	if(RndSpawnPositionDirection == ESpawnDirections::ESD_Top) // Top
	{
		RndScreenLocX = FMath::RandRange(0, ViewportSizeX);		
	}
	else if (RndSpawnPositionDirection == ESpawnDirections::ESD_Bottom) // Down
	{
		RndScreenLocX = FMath::RandRange(0, ViewportSizeX);
		RndScreenLocY = ViewportSizeY;		
	}
	else if (RndSpawnPositionDirection == ESpawnDirections::ESD_Left) // Left
	{		
		RndScreenLocY = FMath::RandRange(0, ViewportSizeY);		
	}
	else // Right
	{
		RndScreenLocX = ViewportSizeX;
		RndScreenLocY = FMath::RandRange(0, ViewportSizeY);
	}
	
	FVector WorldDir;
	UGameplayStatics::GetPlayerController(this,0)->DeprojectScreenPositionToWorld(RndScreenLocX, RndScreenLocY, RndWorldSpawnLoc,WorldDir);

	RndWorldSpawnLoc.Z = 0;

	return RndWorldSpawnLoc;	
}


void AUEAsteroidsCloneGameModeBase::SetScore(const int32 Value)
{
	if(Value <= 0) return;
	
	Score += Value;

	UE_LOG(LogTemp, Warning, TEXT("Score : %i"), Score);

	// TODO: Broadcast score changed event
}

UAsteroidDataAsset* AUEAsteroidsCloneGameModeBase::GetAsteroidDataByType(EAsteroidType AsteroidType) const
{
	UAsteroidDataAsset* Data = nullptr;

	for(const auto DataAsset : AsteroidsDataArray)
	{
		if(DataAsset->AsteroidType == AsteroidType)
		{
			Data = DataAsset;
		}
	}

	return Data;
}

void AUEAsteroidsCloneGameModeBase::PlayerDead()
{
	PlayerCurrentRetryAmount++;
	GetWorldTimerManager().ClearTimer(AsteroidSpawnTimerHandle);	
	
	TArray<AActor*> Asteroids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAsteroid::StaticClass(),Asteroids);

	for(const auto Asteroid : Asteroids)
	{
		Asteroid->Destroy();
	}

	if(PlayerCurrentRetryAmount <= PlayerTotalRetryAmount)
	{
		GetWorldTimerManager().SetTimer(PlayerRetryTimerHandle, this, &AUEAsteroidsCloneGameModeBase::PlayerRetryTimerElapsed,PlayerRetryDelay);			
	}
	else
	{
		//TODO: Show game over UI
		UE_LOG(LogTemp,Warning,TEXT("GAME OVER __ Score %i"), Score);
	}
}

void AUEAsteroidsCloneGameModeBase::PlayerRetryTimerElapsed()
{
	if(ASpaceship* PlayerPawn = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		PlayerPawn->SetActorLocation(FVector::ZeroVector);
		PlayerPawn->OnRetryHandler();

		GetWorldTimerManager().SetTimer(AsteroidSpawnTimerHandle,this,&AUEAsteroidsCloneGameModeBase::SpawnAsteroidTimerElapsed,AsteroidSpawnInterval, true);	
	}	
}


