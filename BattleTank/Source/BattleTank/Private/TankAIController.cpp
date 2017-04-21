// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"

class UTankAimingComponent;


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerTank)) { return; }
	
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius,true,true); // TODO check radius is in cm

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	// Aim towards the player
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	AimingComponent->Fire(); // TODO limit firing rate
	
}