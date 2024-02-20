// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

// Interface class for interactable objects
class ADVENCHARTED_API IInteractable
{
	GENERATED_BODY()

public:
	// Called when the player interacts with the object
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactible")
	void OnInteract(AActor* Interactor, UPrimitiveComponent* HitComponent);

	// Called when the player nears the interactable object and sees the interaction prompt
	UFUNCTION(BlueprintNativeEvent, Category = "Interactible")
	void OnInteractionFound(AActor* Interactor, UPrimitiveComponent* HitComponent);

	// Called when the player moves away from the interactable object and the interaction prompt disappears
	UFUNCTION(BlueprintNativeEvent, Category = "Interactible")
	void OnInteractionLost(AActor* Interactor, UPrimitiveComponent* HitComponent);

	// Called when the player is near the interactable object and can interact with it
	UFUNCTION(BlueprintNativeEvent, Category = "Interactible")
	void OnReadyToInteract(AActor* Interactor, UPrimitiveComponent* HitComponent);

	// Called when the player is near the interactable object but cannot interact with it
	UFUNCTION(BlueprintNativeEvent, Category = "Interactible")
	void OnNotReadyToInteract(AActor* Interactor, UPrimitiveComponent* HitComponent);

	// Returns the interaction definition for the interactable object
	UFUNCTION(BlueprintNativeEvent, Category = "Interactible")
	UInteractionDefinition* GetInteractionDefinition();

	UFUNCTION(BlueprintImplementableEvent, Category = "Advenchanted|Interaction")
	UPrimitiveComponent* GetInteractableComponent();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Advenchanted|Interaction")
	FVector GetInteractableLocation(bool& bWorldLocation);

	UFUNCTION(BlueprintImplementableEvent, Category = "Advenchanted|Interaction")
	FText GetItemName();

	UFUNCTION(BlueprintImplementableEvent, Category = "Advenchanted|Interaction")
	FText GetItemDescription();
};
