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

#pragma endregion 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"));
	float RotationSpeed;
	
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
	void RotateShip(const FInputActionValue& Value);

	void Fire(const FInputActionValue& Value);
	
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABullet> BulletToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BulletSpawnSocketName;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
