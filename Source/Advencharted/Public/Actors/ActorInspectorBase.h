// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/Object.h"
#include "ActorInspectorBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class ADVENCHARTED_API AActorInspectorBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GridInventory")
	TObjectPtr<AActor> ItemActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GridInventory")
	TObjectPtr<UTextureRenderTarget2D> ItemRenderTarget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GridInventory")
	TArray<UPrimitiveComponent*> PrimitiveComponents;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GridInventory")
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GridInventory")
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GridInventory")
	float DevaultFov = 90.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GridInventory")
	FRotator DefaultRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GridInventory")
	FRotator DefaultRotationOffset = FRotator(0.0f, 0.0f, 90.0f);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="GridInventory")
	float CameraDistanceBounceRatio = 2.0f;

public:
	AActorInspectorBase();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category="GIS|Snapper")
	virtual UTextureRenderTarget2D* GetActorSnapshot(AActor* Actor,
	                                                 int32 ResolutionX, int32 ResolutionY, UTextureRenderTarget2D* RenderTarget = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GIS|Snapper")
	void ResetCamera();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GIS|Snapper")
	void RotateCamera(FRotator Delta);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="GIS|Snapper")
	void SetCameraDistance(float NewDistance);

protected:
	float GetDistance() const;

	virtual void PreCaptureSetup(AActor* NewActor);
	void SetupPrimitives(bool bForCapture);
};
