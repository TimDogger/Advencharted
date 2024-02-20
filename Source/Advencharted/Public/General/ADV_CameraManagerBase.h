// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "ADV_CameraManagerBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ADVENCHARTED_API AADV_CameraManagerBase : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Camera")
	float Distance = 100.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Camera")
	float MinDistance = 40.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Camera")
	float LocationInterpSpeed = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Camera")
	float PitchLimit = 89.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Camera")
	FVector CameraRootOffset = FVector(0.0f, 50.0f, 50.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Camera")
	float CameraTraceRadius = 10.0f;

protected:
	FRotator TargetRotation = FRotator(-20.0f, 0.0f, 0.0f);

public:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|Camera")
	void AddRotation(float DeltaYaw, float DeltaPitch);
};
