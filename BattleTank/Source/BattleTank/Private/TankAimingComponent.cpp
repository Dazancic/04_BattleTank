// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
//#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "../Public/TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay() {
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	if (RoundsLeft <= 0) {
		FiringStatus = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
		FiringStatus = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		
		FiringStatus = EFiringState::Aiming;
	}
	else { 
		FiringStatus = EFiringState::Locked;
	}
}


void UTankAimingComponent::AimAt(FVector HitLocation) {
	if (!ensure(Barrel)) { return; }
	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace);
	if (bHaveAimSolution) {
		//UE_LOG(LogTemp, Warning, TEXT("Target location: %s within range"), *HitLocation.ToString());
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		/*auto TankName = GetOwner()->GetName();
		float Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: %s aim solution found for target point: %s with aim direction: %s"), Time, *TankName, *HitLocation.ToString(), *AimDirection.ToString());*/
		MoveBarrelTowards(AimDirection);
	}
	/*else {
		float Time = GetWorld()->GetTimeSeconds();
		auto TankName = GetOwner()->GetName();
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("%f: %s no aim solution found for target point: %s with aim direction: %s"), Time, *TankName, *HitLocation.ToString(), *AimDirection.ToString());
	}*/

}


void UTankAimingComponent::Initialize(UTankBarrel * TankBarrelToSet, UTankTurret * TankTurretToSet)
{
	Barrel = TankBarrelToSet;
	Turret = TankTurretToSet;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDir) {
	if (!ensure(Barrel && Turret)) { return; }

	// Work out difference between current barrel rotation and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDir.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->ElevateBarrel(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Yaw(DeltaRotator.Yaw);
	}
	else {
		Turret->Yaw(-DeltaRotator.Yaw);
	}
}

EFiringState UTankAimingComponent::GetFiringState() const {
	return FiringStatus;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	return !(AimDirection.Equals(Barrel->GetForwardVector(), 0.01));
}

void UTankAimingComponent::Fire() {
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }
	if (FiringStatus == EFiringState::Aiming || FiringStatus == EFiringState::Locked) {
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}

	//Spawn projectile at the socket location on the barrel


}

