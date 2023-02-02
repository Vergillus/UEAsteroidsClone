// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asteroid.h"
#include "AsteroidDataAsset.h"
#include "GameFramework/GameModeBase.h"
#include "UEAsteroidsCloneGameModeBase.generated.h"

class UAsteroidDataAsset;
class UMainHUDWidget;
class UUFODataAsset;
class AEnemyUFO;

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

	int32 Score;	

public:

	AUEAsteroidsCloneGameModeBase();

	void SetScore(const int32 Value);

protected:

	virtual void BeginPlay() override;

	/** Total number to allow player to continue after dead */
	const int32 PlayerTotalRetryAmount = 3;

	/** Current continue/retry number */
	int32 PlayerCurrentRetryAmount;

	/** Spawn interval for asteroid. Used by timer */
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category= "Spawn")
	float AsteroidSpawnInterval;

	/** Spawn interval for UFO. Used by timer */
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category= "Spawn")
	float UFOSpawnInterval;

	/** Value to delay used after players dead to delay player's retry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly , Category= "Spawn")
	float PlayerRetryDelay;

	/** Asteroid class/bp to spawn*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAsteroid> AsteroidToSpawn;	

	/** UFO class/bp to spawn */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyUFO> UFOToSpawn;	

	/** Array that holds asteroid data */
	UPROPERTY(EditAnywhere)
	TArray<UAsteroidDataAsset*> AsteroidsDataArray;

	/** Array that holds UFO data */
	UPROPERTY(EditAnywhere)
	TArray<UUFODataAsset*> UFODataArray;

	/** Main HUD Widget class/bp to create */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "UI")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetToCreate;

	/** Reference to main hud widget */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMainHUDWidget> MainHUDWidgetRef;

	/** Array that holds 4 spawn directions. Used to get random direction */
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

	/** Timer handled to delay players restart/retry progress */
	FTimerHandle PlayerRetryTimerHandle;
#pragma endregion

#pragma region Timer Functions

	UFUNCTION()
	void SpawnAsteroidTimerElapsed();

	UFUNCTION()
	void SpawnUFOTimerElapsed();

	UFUNCTION()
	void PlayerRetryTimerElapsed();
#pragma endregion

	/** Returns random spawn point */
	FVector GetRandomSpawnPoint() const;

	/* Spawns asteroids at random positions */
	UFUNCTION(BlueprintCallable)
	void SpawnAsteroid() const;

	/** Spawn UFO at random position */
	UFUNCTION(BlueprintCallable)
	void SpawnUFO();
	
public:	

	/* Spawns given number of  asteroid(s) at given position. Called via Asteroid when it is dead */	
	UFUNCTION(BlueprintCallable)
	void SpawnAsteroidAtPosition(EAsteroidType AsteroidType, FVector SpawnPosition ,int Amount = 1);

	/** Returns Asteroid Data Asset by AsteroidType */
	UFUNCTION(BlueprintCallable)
	UAsteroidDataAsset* GetAsteroidDataByType(EAsteroidType AsteroidType) const;

	/** Resets the scene by removing all spawned objects in the world. */
	UFUNCTION(BlueprintCallable)
	void ResetScene() const;

	/* Called via player ship when it is dead **/
	UFUNCTION(BlueprintCallable)
	void PlayerDead();
	
};
