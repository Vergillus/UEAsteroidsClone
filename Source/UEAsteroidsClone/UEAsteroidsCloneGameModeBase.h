// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asteroid.h"
#include "AsteroidDataAsset.h"
#include "GameFramework/GameModeBase.h"
#include "UEAsteroidsCloneGameModeBase.generated.h"

class UAsteroidDataAsset;

UENUM(BlueprintType)
enum class ESpawnDirections : uint8
{
	ESD_Top,
	ESD_Bottom,
	ESD_Left,
	ESD_Right,

	ESD_NONE
};

/**
 * 
 */
UCLASS()
class UEASTEROIDSCLONE_API AUEAsteroidsCloneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	static int32 Score;
	

public:

	AUEAsteroidsCloneGameModeBase();

	static void SetScore(const int32 Value);

protected:

	virtual void BeginPlay() override;
	
	const int32 PlayerTotalRetryAmount = 3;
	int32 PlayerCurrentRetryAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category= "Spawn")
	float AsteroidSpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category= "Spawn")
	float UFOSpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category= "Spawn")
	float PlayerRetryDelay;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAsteroid> AsteroidToSpawn;	
	
	UPROPERTY(EditAnywhere)
	TArray<UAsteroidDataAsset*> AsteroidsDataArray;

	const TArray<ESpawnDirections> SpawnDirections
	{
		ESpawnDirections::ESD_Top,
		ESpawnDirections::ESD_Bottom,
		ESpawnDirections::ESD_Left,
		ESpawnDirections::ESD_Right
	};

#pragma region Timer Handles

	FTimerHandle AsteroidSpawnTimerHandle;
	FTimerHandle UFOSpawnTimerHandle;
	FTimerHandle PlayerRetryTimerHandle;
#pragma endregion

#pragma region Timer Functions

	UFUNCTION()
	void SpawnAsteroidTimerElapsed();

	UFUNCTION()
	void PlayerRetryTimerElapsed();
#pragma endregion 

	
	// TODO: UFO Spawner timer
	// TODO: UI events
	

	FVector GetRandomSpawnPoint() const;

public:

	/* Spawns asteroids at random positions */
	UFUNCTION(BlueprintCallable)
	void SpawnAsteroid(UAsteroidDataAsset* AsteroidData) const;

	/* Spawns given number of  asteroid(s) at given position */	
	UFUNCTION(BlueprintCallable)
	void SpawnAsteroidAtPosition(UAsteroidDataAsset* AsteroidData, FVector SpawnPosition ,int Amount = 1);

	UFUNCTION(BlueprintCallable)
	UAsteroidDataAsset* GetAsteroidDataByType(EAsteroidType AsteroidType) const;

	/* Called via player ship when it is dead **/
	UFUNCTION(BlueprintCallable)
	void PlayerDead();
	
};
