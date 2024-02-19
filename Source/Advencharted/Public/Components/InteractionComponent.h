// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "General/ADV_HUD_Base.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionUpdated, UPrimitiveComponent*, HitComponent);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVENCHARTED_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float InteractionRadius = 500.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
	TSet<UPrimitiveComponent*> InteractableComponents;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float InteractionTraceLength = 200.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float InteractionTraceRadius = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float CameraLeftOffset = 10.0f;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionUpdated OnCurrentInteractionUpdated;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	FOnInteractionUpdated OnInteractionFound;

	UPROPERTY(BlueprintReadWrite, Category = "Interaction")
	FOnInteractionUpdated OnInteractionLost;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	bool bDrawDebug = false;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TEnumAsByte<ETraceTypeQuery> TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

private:
	TObjectPtr<UPrimitiveComponent> CurrentInteraction = nullptr;

public:
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateInteraction();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent)
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Interaction")
	UPrimitiveComponent* GetCurrentInteraction() const { return CurrentInteraction; }
	
	virtual void BeginPlay() override;
	void CreateIntearctionSphere();
};
