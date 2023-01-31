// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UEASTEROIDSCLONE_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void ScoreChanged(const int32 Score);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerDead(const int32 RetryAmount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(const int32 Score);
	
};
