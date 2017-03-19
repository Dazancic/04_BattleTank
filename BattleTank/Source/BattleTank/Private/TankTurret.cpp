// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "../Public/TankTurret.h"


void UTankTurret::Yaw(float RelativeSpeed) {
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto YawChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewYaw = RelativeRotation.Yaw + YawChange;

	//auto NewYaw = FMath::Clamp<float>(RawNewYaw, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(0.f, RawNewYaw, 0.f));
}

