// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "tank.h"
#include "TankAIController.h"



ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

void ATankAIController::BeginPlay() {
	Super::BeginPlay();
	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerTank not found"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found PlayerTank named: %s"), *PlayerTank->GetName());
	}
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// TODO Move Towards the player
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

	// Fire when ready
}

ATank* ATankAIController::GetPlayerTank() const {
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	return Cast<ATank>(PlayerPawn);
}
