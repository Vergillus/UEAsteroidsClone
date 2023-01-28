// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UEAsteroidsCloneGameModeBase.generated.h"

class UAsteroidDataAsset;

/**
 * 
 */
UCLASS()
class UEASTEROIDSCLONE_API AUEAsteroidsCloneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	static int32 Score;
	

public:

	static void SetScore(const int32 Value);

protected:

	virtual void BeginPlay() override;
	
	const int32 PlayerTotalRetryAmount = 3;
	int32 PlayerCurrentRetryAmount = 0;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UAsteroidDataAsset>> AsteroidsDataArray;

	// TODO: Asteroids Spawner timer
	// TODO: UFO Spawner timer
	// TODO: UI events
	// TODO: Player dead function
	//void SpawnAsteroid(UAsteroidDataAsset AsteroidData);
	
};
