// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidgetBase.generated.h"

class UImage;
class UCanvasPanelSlot;

UCLASS(Abstract, Blueprintable, BlueprintType)
class ADVENCHARTED_API UInteractionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|InteractionWidgetBase")
	TObjectPtr<UImage> InteractionImage;

protected:
	UPROPERTY()
	TObjectPtr<AActor> InteractableActor = nullptr;

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot = nullptr;

public:
	UInteractionWidgetBase(const FObjectInitializer& ObjectInitializer);
	UCanvasPanelSlot* GetCanvasPanelSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|InteractionWidgetBase")
	AActor* GetInteractableActor();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Advenchanted|InteractionWidgetBase")
	void InitializeInteractionWidget(AActor* Actor);
};
