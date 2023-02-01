// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

class UInputAction;
class USphereComponent;
class UStaticMeshComponent;
class UFloatingPawnMovement;
class UInputMappingContext;
class UScreenWarper;
class ABullet;
class UNiagaraComponent;

UCLASS()
class UEASTEROIDSCLONE_API ASpaceship : public APawn
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SpaceshipMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category= "Movement")
	TObjectPtr<UFloatingPawnMovement> PawnMovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true") , Category= "Input")
	UInputMappingContext* SpaceshipMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScreenWarper> ScreenWarperComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> ShipAudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> SpaceshipThrusterNiagaraComponent;

#pragma endregion 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float BulletSpeed;

	const FName PlayerBulletTagName = "PlayerBullet";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=( AllowPrivateAccess = "true"))
	UMaterialInterface* BulletMaterial;
	
public:
	// Sets default values for this pawn's properties
	ASpaceship();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

#pragma region Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Input")
	UInputAction* RotateAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =  "Input")
	UInputAction* FireAction;

#pragma endregion

#pragma region Input Action Functions

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void MoveEnd(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void RotateShip(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);
	
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABullet> BulletToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BulletSpawnSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX | Sound")
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX | Sound")
	TObjectPtr<USoundBase> ExplosionSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "VFX | Particle")
	TObjectPtr<UParticleSystem> ExplosionParticle;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when hit by an asteroid or a bullet */
	UFUNCTION(BlueprintCallable)
	void OnDeathHandler();	

public:

	/** Called via GM after dead if the player has not used all retry points  */
	void OnRetryHandler();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
