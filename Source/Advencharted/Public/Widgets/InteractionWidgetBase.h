// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InteractionWidgetBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class ADVENCHARTED_API UInteractionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UImage> InteractionImage;

protected:
	UPROPERTY()
	TObjectPtr<AActor> InteractableActor = nullptr;

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot = nullptr;

public:
	UInteractionWidgetBase(const FObjectInitializer& ObjectInitializer);
	UCanvasPanelSlot* GetCanvasPanelSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetInteractableActor();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Advencharted|InteractionWidgetBase")
	void InitializeInteractionWidget(AActor* Actor);
};
