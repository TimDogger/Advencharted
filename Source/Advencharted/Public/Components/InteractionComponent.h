// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionUpdated, UPrimitiveComponent*, HitComponent);

class USphereComponent;
class UPrimitiveComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVENCHARTED_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Advenchanted|Interaction")
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Interaction")
	float InteractionRadius = 500.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|Interaction")
	TSet<UPrimitiveComponent*> InteractableComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Interaction")
	float InteractionTraceLength = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Interaction")
	float InteractionTraceRadius = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Interaction")
	float CameraLeftOffset = 10.0f;

	UPROPERTY(BlueprintAssignable, Category = "Advenchanted|Interaction")
	FOnInteractionUpdated OnCurrentInteractionUpdated;

	UPROPERTY(BlueprintReadWrite, Category = "Advenchanted|Interaction")
	FOnInteractionUpdated OnInteractionFound;

	UPROPERTY(BlueprintReadWrite, Category = "Advenchanted|Interaction")
	FOnInteractionUpdated OnInteractionLost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Advenchanted|Interaction")
	bool bDrawDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|Interaction")
	TEnumAsByte<ETraceTypeQuery> TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

private:
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CurrentInteraction = nullptr;

public:
	UInteractionComponent();	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateInteraction();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Advenchanted|Interaction")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Advenchanted|Interaction")
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|Interaction")
	void Interact();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|Interaction")
	UPrimitiveComponent* GetCurrentInteraction() const { return CurrentInteraction; }

private:
	void CreateIntearctionSphere();
};
