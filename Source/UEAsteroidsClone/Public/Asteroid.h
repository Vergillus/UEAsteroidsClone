// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asteroid.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UAsteroidDataAsset;
class UScreenWarper;
class ULifespanController;

UCLASS()
class UEASTEROIDSCLONE_API AAsteroid : public AActor
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> AsteroidMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScreenWarper> ScreenWarperComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULifespanController> LifespanController;
#pragma endregion 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	UAsteroidDataAsset* DataAsset;
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

	/** Finishes the asteroid creation */
	void InitializeAsteroid(UAsteroidDataAsset* Data);

protected:
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	TObjectPtr<UParticleSystem> DeadParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	TObjectPtr<USoundBase> ExplosionSound;
};
