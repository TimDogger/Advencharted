// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "General/InteractionDefinition.h"
#include "InteractionWidgetBase.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType)
class ADVENCHARTED_API UInteractionWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UImage> InteractionImage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UTextBlock> InteractionTextBlock;

protected:
	UPROPERTY()
	TObjectPtr<AActor> InteractableActor = nullptr;

	UPROPERTY()
	TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot = nullptr;

public:
	UInteractionWidgetBase(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Advencharted|InteractionWidgetBase")
	void InitializeInteractionWidget(const UInteractionDefinition* InteractionDefinition, AActor* Actor);

	AActor* GetInteractableActor() const;
	void UpdatePosition();
};
