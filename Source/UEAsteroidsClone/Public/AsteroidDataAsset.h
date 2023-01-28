// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AsteroidDataAsset.generated.h"

UENUM(BlueprintType)
enum class EAsteroidType : uint8
{
	EAT_Small UMETA(DisplayName = "Small"),
	EAT_Medium UMETA(DisplayName = "Medium"),
	EAT_Large UMETA(DisplayName = "Large"),

	EAT_NONE UMETA(DisplayName = "None")
};

/**
 * 
 */
UCLASS()
class UEASTEROIDSCLONE_API UAsteroidDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	/* Type of the asteroid */
	UPROPERTY(EditAnywhere);
	EAsteroidType AsteroidType;

	/* Uniform scale of the asteroid */
	UPROPERTY(EditAnywhere);
	float UniformScale;

	/* Score to receive when asteroid is destroyed */
	UPROPERTY(EditAnywhere);
	int32 ReceivedScore;
	
};
