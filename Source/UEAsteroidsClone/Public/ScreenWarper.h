// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenWarper.generated.h"

UENUM()
enum class EWarpAxis { EWA_X, EWA_Y };

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEASTEROIDSCLONE_API UScreenWarper : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScreenWarper();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TObjectPtr<APlayerController> PlayerController;

	FVector2D ScreenLocation;

	int32 ViewportSizeX;
	int32 ViewportSizeY;

	FVector WorldLocation, WorldDirection;

	void WarpOwner(EWarpAxis WarpAxis);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
