// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TankAIController.generated.h"


/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float AcceptanceRadius = 3000;

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	// How close can the AI tank get
	
};