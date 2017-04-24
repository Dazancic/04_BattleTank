// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTrack.h"
#include "../Public/TankMovementComponent.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// No need to call Super as we're replacing the functionality
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	
	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	//IntendMoveForward(ForwardThrow);
	auto RotateThrow = FVector::CrossProduct(TankForward,AIForwardIntention).Z;
	//UE_LOG(LogTemp, Warning, TEXT("Turning: %f"), RotateThrow);
	//IntendTurnRight(RotateThrow);
	IntendMove(ForwardThrow, RotateThrow);
}

void UTankMovementComponent::IntendMove(float ControllerY, float ControllerX) {
	ControllerX *= -1;
	float V = (100 - FMath::Abs<float>(ControllerX)) * (ControllerY / 100.f) + ControllerY;
	float W = (100 - FMath::Abs<float>(ControllerY)) * (ControllerX / 100.f) + ControllerX;
	float RightThrottle = (V + W) / 2.f;
	float LeftThrottle = (V - W) / 2.f;
	//UE_LOG(LogTemp, Warning, TEXT("Sending movement: Y: %f, X: %f, L: %f, R: %f"), ControllerY, ControllerX, LeftThrottle, RightThrottle);
	LeftTrack->SetThrottle(LeftThrottle);
	RightTrack->SetThrottle(RightThrottle);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}