// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifespanController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEASTEROIDSCLONE_API ULifespanController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifespanController();

protected:
	
	FTimerHandle LifespanTimerHandle;

	/** Function for lifespan timer*/
	UFUNCTION(BlueprintCallable)
	void LifespanTimerElapsed();

	/** Function to bind OnActorWarped  event/delegate in UScreenWarper component.
	 * Called when the owner actor is warped/teleported via UScreenWarper component. 
	 */
	UFUNCTION(BlueprintCallable)
	void ActorWarpedHandler();

	/** Variable to decide whether we can destroy owner actor after warping is done */
	bool bCanDestroyActor;

	float Lifespan;
	
public:

	UFUNCTION(BlueprintCallable)
	void SetLifespan(const float NewLifespan);
};
