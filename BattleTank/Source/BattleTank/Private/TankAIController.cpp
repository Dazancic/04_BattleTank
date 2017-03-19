// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "tank.h"
#include "../Public/TankAIController.h"



/*ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}*/

void ATankAIController::BeginPlay() {
	Super::BeginPlay();
	
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// TODO Move Towards the player
	//UE_LOG(LogTemp, Warning, TEXT("Tick"));
	if (!ControlledTank) {
		ControlledTank = Cast<ATank>(GetPawn());
	}
	
	if (PlayerTank) {
		
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		// Fire when ready
		ControlledTank->Fire();
	}
	else {
		auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (PlayerPawn) {
			PlayerTank = Cast<ATank>(PlayerPawn);
		}
	}
}

/*ATank* ATankAIController::GetPlayerTank() const {
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	return Cast<ATank>(PlayerPawn);
}*/
