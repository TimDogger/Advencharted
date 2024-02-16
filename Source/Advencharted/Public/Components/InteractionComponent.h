// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "General/ADV_HUD_Base.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVENCHARTED_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
	float InteractionRadius = 300.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
	TSet<AActor*> InteractableActors;

public:
	UInteractionComponent();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact(AActor* TargetActor);
	
	virtual void BeginPlay() override;
	void CreateIntearctionSphere();

	virtual void OnInteractionFound(AActor* Actor);
	virtual void OnInteractionLost(AActor* Actor);
	AADV_HUD_Base* GetHUD();
};
