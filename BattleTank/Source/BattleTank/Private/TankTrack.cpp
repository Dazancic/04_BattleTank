// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "../Public/TankTrack.h"


void UTankTrack::SetThrottle(float Throttle) {
	auto Name = GetName();
	

	// TODO clamp actual throttle value so player can't change speed
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	//UE_LOG(LogTemp, Warning, TEXT("%s force: %s"), *Name, *ForceApplied.ToString());
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

