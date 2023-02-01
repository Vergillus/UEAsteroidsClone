// Fill out your copyright notice in the Description page of Project Settings.


#include "LifespanController.h"

#include "ScreenWarper.h"

// Sets default values for this component's properties
ULifespanController::ULifespanController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void ULifespanController::LifespanTimerElapsed()
{
	bCanDestroyActor = true;
}

void ULifespanController::ActorWarpedHandler()
{
	// Owner actor is warped and it lifespan is over we can destroy the owner out of the screen.
	if(bCanDestroyActor)
	{
		GetOwner()->Destroy();
	}
}

void ULifespanController::SetLifespan(const float NewLifespan)
{
	Lifespan = NewLifespan;

	// Get the UScreenWarper component
	if(UScreenWarper* WarperComp = GetOwner()->FindComponentByClass<UScreenWarper>())
	{
		// Subscribe to OnActorWarped event
		WarperComp->OnActorWarped.AddDynamic(this, &ULifespanController::ActorWarpedHandler);

		// Start Lifespan timer.
		GetWorld()->GetTimerManager().SetTimer(LifespanTimerHandle, this, &ULifespanController::LifespanTimerElapsed, Lifespan);		
	}		
}


