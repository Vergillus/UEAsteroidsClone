// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEAsteroidsCloneGameModeBase.h"

#include "EnemyUFO.h"
#include "Spaceship.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "MainHUDWidget.h"
#include "UFODataAsset.h"


AUEAsteroidsCloneGameModeBase::AUEAsteroidsCloneGameModeBase() :
	Score(0),
	PlayerCurrentRetryAmount(0),
	AsteroidSpawnInterval(1),
	UFOSpawnInterval(1),
	PlayerRetryDelay(1)
{
}


void AUEAsteroidsCloneGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if(MainHUDWidgetToCreate)
	{
		MainHUDWidgetRef = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDWidgetToCreate);
		MainHUDWidgetRef->AddToViewport();
	}

	if(AsteroidToSpawn)
	{
		constexpr  float InFirstDelay = 1.0f;
		GetWorldTimerManager().SetTimer(AsteroidSpawnTimerHandle,this,&AUEAsteroidsCloneGameModeBase::SpawnAsteroidTimerElapsed,AsteroidSpawnInterval, true, InFirstDelay);		
	}

	if(UFOToSpawn)
	{
		GetWorldTimerManager().SetTimer(UFOSpawnTimerHandle, this, &AUEAsteroidsCloneGameModeBase::SpawnUFOTimerElapsed, UFOSpawnInterval, true);		
	}
	
}

void AUEAsteroidsCloneGameModeBase::SpawnAsteroidTimerElapsed()
{
	SpawnAsteroid(AsteroidsDataArray[FMath::RandRange(0, AsteroidsDataArray.Num() - 1)]);
}

void AUEAsteroidsCloneGameModeBase::SpawnUFOTimerElapsed()
{
	SpawnUFO();
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

void AUEAsteroidsCloneGameModeBase::SpawnAsteroidAtPosition(EAsteroidType AsteroidType, FVector SpawnPosition,
	int Amount)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UAsteroidDataAsset* AsteroidData =  GetAsteroidDataByType(AsteroidType);

	if(AsteroidData == nullptr)
	{
		return;
	}

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


void AUEAsteroidsCloneGameModeBase::SpawnUFO()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UUFODataAsset* UFOData = UFODataArray[FMath::RandRange(0, UFODataArray.Num() - 1)];

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetRandomSpawnPoint());
	SpawnTransform.SetScale3D(FVector::One() * UFOData->UniformScale);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	if (const auto UFOClone = GetWorld()->SpawnActor<AEnemyUFO>(UFOToSpawn,SpawnTransform,SpawnParameters))
	{
		UFOClone->InitializeUFO(UFOData);
	}
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

	// Notify UI that the score is changed.
	if(MainHUDWidgetRef)
	{
		MainHUDWidgetRef->ScoreChanged(Score);		
	}
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

	// Stop Asteroid Spawn timer.
	GetWorldTimerManager().ClearTimer(AsteroidSpawnTimerHandle);

	// Stop UFO Spawn timer.
	GetWorldTimerManager().ClearTimer(UFOSpawnTimerHandle);

	// Find and destroy all asteroids in the world
	//-----------------------------------------------
	TArray<AActor*> Asteroids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AAsteroid::StaticClass(),Asteroids);

	for(const auto Asteroid : Asteroids)
	{
		Asteroid->Destroy();
	}
	//-----------------------------------------------

	// Find and destroy all UFO(s) in the world
	//-----------------------------------------------
	TArray<AActor*> UFOs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AEnemyUFO::StaticClass(), UFOs);
	for (const auto UFO : UFOs)
	{
		UFO->Destroy();
	}
	//-----------------------------------------------
	
	
	// Notify UI that player is dead
	if(MainHUDWidgetRef)
	{
		MainHUDWidgetRef->PlayerDead(PlayerCurrentRetryAmount);
	}		

	if(PlayerCurrentRetryAmount < PlayerTotalRetryAmount) // Check if the player has enough retry points;
	{
		// Restore player after some delay
		GetWorldTimerManager().SetTimer(PlayerRetryTimerHandle, this, &AUEAsteroidsCloneGameModeBase::PlayerRetryTimerElapsed,PlayerRetryDelay);		
	}
	else // Game over
	{
		if(MainHUDWidgetRef)
		{
			MainHUDWidgetRef->GameOver(Score);
		}		
	}
}

void AUEAsteroidsCloneGameModeBase::PlayerRetryTimerElapsed()
{
	if(ASpaceship* PlayerPawn = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		PlayerPawn->SetActorRotation(FQuat::Identity);
		PlayerPawn->SetActorLocation(FVector::ZeroVector);
		PlayerPawn->OnRetryHandler();

		// Restart asteroid spawn timer
		GetWorldTimerManager().SetTimer(AsteroidSpawnTimerHandle,this,&AUEAsteroidsCloneGameModeBase::SpawnAsteroidTimerElapsed,AsteroidSpawnInterval, true);

		// Restart UFO spawn timer
		GetWorldTimerManager().SetTimer(UFOSpawnTimerHandle,this,&AUEAsteroidsCloneGameModeBase::SpawnUFOTimerElapsed,UFOSpawnInterval, true);
	}	
}


