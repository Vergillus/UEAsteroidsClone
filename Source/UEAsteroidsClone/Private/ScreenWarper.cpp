// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenWarper.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UScreenWarper::UScreenWarper()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UScreenWarper::BeginPlay()
{
	Super::BeginPlay();

	// Delay PlayerController and viewport sizes initialization to prevent teleporting the spaceship to wrong screen position bug
	// at the start when playing stand-alone mode.
	//--------------------------------------------------------
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	constexpr float Delay = 0.1f;
	TimerDelegate.BindLambda([&]
	{
		PlayerController = UGameplayStatics::GetPlayerController(GetOwner(),0);
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);		
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate,Delay,false);
	//--------------------------------------------------------	
}

// Called every frame
void UScreenWarper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	

	// Get the owners screen position and check if it X and/or Y component satisfies the conditions below.
	// If so teleport the owner to opposite side of the screen
	if (PlayerController)
	{
		PlayerController->ProjectWorldLocationToScreen(GetOwner()->GetActorLocation(), ScreenLocation);		

		if (ScreenLocation.X > ViewportSizeX)
		{
			PlayerController->DeprojectScreenPositionToWorld(0,ViewportSizeY,WorldLocation, WorldDirection);
			WarpOwner(EWarpAxis::EWA_X);			
		}
		else if(ScreenLocation.X < 0)
		{
			PlayerController->DeprojectScreenPositionToWorld(ViewportSizeX,ViewportSizeY,WorldLocation, WorldDirection);
			WarpOwner(EWarpAxis::EWA_X);
		}

		if (ScreenLocation.Y > ViewportSizeY)
		{
			PlayerController->DeprojectScreenPositionToWorld(ViewportSizeX,0,WorldLocation, WorldDirection);
			WarpOwner(EWarpAxis::EWA_Y);
		}
		else if (ScreenLocation.Y < 0)
		{
			PlayerController->DeprojectScreenPositionToWorld(ViewportSizeX,ViewportSizeY,WorldLocation, WorldDirection);
			WarpOwner(EWarpAxis::EWA_Y);
		}
	}

	// ...
}

void UScreenWarper::WarpOwner(EWarpAxis WarpAxis) const
{
	if (WarpAxis == EWarpAxis::EWA_X)
	{
		const FVector NewLocation{GetOwner()->GetActorLocation().X, WorldLocation.Y, GetOwner()->GetActorLocation().Z};
		GetOwner()->SetActorLocation(NewLocation);
	}
	else
	{
		const FVector NewLocation{WorldLocation.X, GetOwner()->GetActorLocation().Y, GetOwner()->GetActorLocation().Z};
		GetOwner()->SetActorLocation(NewLocation);
	}

	if(OnActorWarped.IsBound())
	{
		OnActorWarped.Broadcast();
	}
}
