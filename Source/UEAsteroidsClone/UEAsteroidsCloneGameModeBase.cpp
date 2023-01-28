// Copyright Epic Games, Inc. All Rights Reserved.


#include "UEAsteroidsCloneGameModeBase.h"

int32 AUEAsteroidsCloneGameModeBase::Score = 0;

void AUEAsteroidsCloneGameModeBase::BeginPlay()
{
	Score = 0;
	Super::BeginPlay();
}

void AUEAsteroidsCloneGameModeBase::SetScore(const int32 Value)
{
	if(Value <= 0) return;
	
	Score += Value;

	// TODO: Broadcast score changed event
}

