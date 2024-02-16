// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactible.generated.h"

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
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactible")
	void OnInteract();

	// Called when the player is near the interactable object
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactible")
	void OnInteractionFound();

	// Called when the player is no longer near the interactable object
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactible")
	void OnInteractionLost();

	// Returns true if the player can interact with the object
	UFUNCTION(BlueprintImplementableEvent, Category = "Interactible")
	bool CanInteract();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interactible")
	UInteractionDefinition* GetInteractionDefinition();
};
