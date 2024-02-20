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
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Advenchanted|Inspector")
	TObjectPtr<AActor> ItemActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Advenchanted|Inspector")
	TObjectPtr<UTextureRenderTarget2D> ItemRenderTarget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Advenchanted|Inspector")
	TArray<UPrimitiveComponent*> PrimitiveComponents;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Advenchanted|Inspector")
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Advenchanted|Inspector")
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Advenchanted|Inspector")
	float DevaultFov = 90.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Advenchanted|Inspector")
	FRotator DefaultRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Advenchanted|Inspector")
	FRotator DefaultRotationOffset = FRotator(0.0f, 0.0f, 90.0f);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Advenchanted|Inspector")
	float CameraDistanceBounceRatio = 2.0f;

public:
	AActorInspectorBase();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category="Advenchanted|Inspector")
	virtual UTextureRenderTarget2D* GetActorSnapshot(AActor* Actor,
	                                                 int32 ResolutionX, int32 ResolutionY, UTextureRenderTarget2D* RenderTarget = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Advenchanted|Inspector")
	void ResetCamera();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Advenchanted|Inspector")
	void RotateCamera(FRotator Delta);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Advenchanted|Inspector")
	void SetCameraDistance(float NewDistance);

protected:
	float GetDistance() const;

	virtual void PreCaptureSetup(AActor* NewActor);
	void SetupPrimitives(bool bForCapture);
};
