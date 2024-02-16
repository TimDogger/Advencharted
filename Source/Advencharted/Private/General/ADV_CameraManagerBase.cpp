// Fill out your copyright notice in the Description page of Project Settings.


#include "General/ADV_CameraManagerBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void AADV_CameraManagerBase::UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime)
{
	FVector CurrentLocation = OutVT.Target->GetActorLocation();
	FVector X = OutVT.Target->GetActorForwardVector() * CameraRootOffset.X;
	FVector Y = OutVT.Target->GetActorRightVector() * CameraRootOffset.Y;
	FVector Z = OutVT.Target->GetActorUpVector() * CameraRootOffset.Z;
	FVector StartTraceLocation = CurrentLocation + X + Y + Z;

	FRotator ControlRotation = GetOwningPlayerController()->GetControlRotation();
	ControlRotation.Yaw += 180;
	if (UKismetMathLibrary::InRange_FloatFloat(ControlRotation.Pitch, 0, 180))
	{
		ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, -PitchLimit, PitchLimit);
	}
	else
	{
		ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, 360 - PitchLimit, 360 + PitchLimit);	
	}
	FVector EndTraceLocation = UKismetMathLibrary::GetForwardVector(ControlRotation) * Distance + StartTraceLocation;
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(EndTraceLocation, StartTraceLocation);
	
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore = {OutVT.Target};
	UKismetSystemLibrary::SphereTraceSingle(this, StartTraceLocation, EndTraceLocation, CameraTraceRadius,
	                                        UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore,
	                                        EDrawDebugTrace::None, HitResult, true);

	OutVT.POV.Rotation = CameraRotation;
	FVector NewLocation = HitResult.bBlockingHit ? HitResult.Location : EndTraceLocation;
	OutVT.POV.Location = FMath::VInterpTo(GetCameraLocation(), NewLocation, DeltaTime, LocationInterpSpeed);
	FTransform CameraTransform = FTransform(CameraRotation, OutVT.POV.Location);
	OutVT.POV.Location += CameraTransform.TransformVector(CameraOffset);
}

void AADV_CameraManagerBase::AddRotation(float DeltaYaw, float DeltaPitch)
{
	TargetRotation.Yaw += DeltaYaw;
	TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch + DeltaPitch, -PitchLimit, PitchLimit);
}
