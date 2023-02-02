// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UScreenWarper;

UCLASS()
class UEASTEROIDSCLONE_API ABullet : public AActor
{
	GENERATED_BODY()

#pragma region Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> RootComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BulletMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScreenWarper> ScreenWarperComp;
#pragma endregion 
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE TObjectPtr<USphereComponent> GetRootCollider() const { return RootComp;}

	/** Finalizes the bullet spawn process */
	UFUNCTION(BlueprintCallable)
	void InitializeBullet(FVector Velocity, UMaterialInterface* BulletMat = nullptr);

protected:

	/** Lifespan of the bullet */
	UPROPERTY(EditDefaultsOnly)
	float LifeTime;
};
