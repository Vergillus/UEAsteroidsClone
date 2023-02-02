// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyUFO.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UScreenWarper;
class UAudioComponent;
class UUFODataAsset;
class ULifespanController;

UCLASS()
class UEASTEROIDSCLONE_API AEnemyUFO : public AActor
{
	GENERATED_BODY()

#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> UFOMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<UScreenWarper> ScreenWarperComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<ULifespanController> LifespanController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> UFOAudioComp;
#pragma endregion 

	const FName EnemyBulletTagName = "EnemyBullet";
	
public:	
	// Sets default values for this actor's properties
	AEnemyUFO();

	void InitializeUFO(UUFODataAsset* DataAsset);

protected:	

	FTimerHandle AttackCooldownTimerHandle;

	UPROPERTY()
	UUFODataAsset* UFOData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	TObjectPtr<UParticleSystem> DeadParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX")
	TObjectPtr<USoundBase> ExplosionSound;

	UFUNCTION()
	void CooldownTimerElapsed();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
