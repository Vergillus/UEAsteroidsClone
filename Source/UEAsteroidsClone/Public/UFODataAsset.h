// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UFODataAsset.generated.h"

class ABullet;

UENUM(BlueprintType)
enum class EUFOType : uint8
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
class UEASTEROIDSCLONE_API UUFODataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** Type of the asteroid */
	UPROPERTY(EditAnywhere);
	EUFOType UFOType;

	/** Uniform scale of the asteroid */
	UPROPERTY(EditAnywhere);
	float UniformScale;

	/** Score to receive when asteroid is destroyed */
	UPROPERTY(EditAnywhere);
	int32 ReceivedScore;

	/** The life time of UFO object */
	UPROPERTY(EditAnywhere);
	float LifeSpan;

	/** Minimum UFO speed */
	UPROPERTY(EditAnywhere);
	float UFOMinSpeed;

	/** Maximum UFO speed */
	UPROPERTY(EditAnywhere);
	float UFOMaxSpeed;

	/** Duration to wait after attack initiated */
	UPROPERTY(EditAnywhere);
	float AttackCooldownDuration;

	/** Bullet blueprint to spawn */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABullet> BulletToSpawn;

	/** Speed of the Bullet */
	UPROPERTY(EditDefaultsOnly)
	float BulletSpeed;

	
	
};
