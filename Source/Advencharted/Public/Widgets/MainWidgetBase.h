// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "UObject/Object.h"
#include "MainWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class ADVENCHARTED_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UCanvasPanel> MainCanvas;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class UInteractionKeyWidgetBase> InteractionKeyWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class UActorInspectorWidgetBase> InspectorWidget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	TMap<TObjectPtr<AActor>, TObjectPtr<UInteractionWidgetBase>> InteractionWidgets;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class AADV_MainCharacterBase> MainCharacter;

public:
	UMainWidgetBase(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnInspectorVisibilityChanged(ESlateVisibility InVisibility);
	
	void InspectItem(AActor* Actor);

	UFUNCTION()
	void ShowInteractionWidgetFor(UPrimitiveComponent* InteractionComponent);

	UFUNCTION()
	void RemoveInteractionWidgetFor(UPrimitiveComponent* InteractionComponent);
	
	UFUNCTION()
	void OnCurrentInteractionUpdated(UPrimitiveComponent* HitComponent);
	
};
