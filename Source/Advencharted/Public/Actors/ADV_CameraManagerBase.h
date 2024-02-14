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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float Distance = 400.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float RotationSpeed = 20.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float PitchLimit = 80.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	FVector CameraRootOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	FVector CameraOffset = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	float CameraTraceRadius = 10.0f;

protected:
	UPROPERTY()
	FRotator TargetRotation = FRotator(-20.0f, 0.0f, 0.0f);

public:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void AddRotation(float DeltaYaw, float DeltaPitch);
};
