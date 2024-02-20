// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDefinition.generated.h"

class UInteractionWidgetBase;

UCLASS(Blueprintable, EditInlineNew)
class UInteractionDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advenchanted|Interaction")
	FText InteractionText = FText::FromString("Interact");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Advenchanted|Interaction")
	TSubclassOf<UInteractionWidgetBase> InteractionWidgetClass;

public:
	// Called when the interaction is triggered. This is a blueprint implementable event, so it can be overridden in blueprints. 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Advenchanted|Interaction")
	void OnInteract(AActor* Interactor, AActor* Interactable);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|Interaction")
	bool IsValid();
};

