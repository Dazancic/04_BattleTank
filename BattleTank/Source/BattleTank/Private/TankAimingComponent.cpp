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
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
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

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection) {
	if (!ensure(Barrel && Turret)) { return; }

	// Work out difference between current barrel rotation and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->ElevateBarrel(DeltaRotator.Pitch);
	Turret->Yaw(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire() {
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (isReloaded) {
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}

	//Spawn projectile at the socket location on the barrel


}

