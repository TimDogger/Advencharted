// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/ActorInspectorWidgetBase.h"
#include "Widgets/InteractionWidgetBase.h"
#include "Widgets/MainWidgetBase.h"
#include "ADV_HUD_Base.generated.h"

UCLASS(Abstract)
class ADVENCHARTED_API AADV_HUD_Base : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD|Settings")
	TSubclassOf<UMainWidgetBase> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD|Settings")
	TSubclassOf<UInteractionWidgetBase> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD|Settings")
	TSubclassOf<UActorInspectorWidgetBase> InspectorWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Advenchanted|HUD")
	TObjectPtr<UMainWidgetBase> MainWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Advenchanted|HUD")
	TArray<TObjectPtr<UInteractionWidgetBase>> InteractionWidgets;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD", Instanced)
	TObjectPtr<UInteractionDefinition> DefaultInteractionDefinition;

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Advenchanted|HUD")
	void ShowInteractionWidgetFor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|HUD")
	void RemoveInteractionWidgetFor(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|HUD")
	UInteractionWidgetBase* GetInteractionWidgetFor(AActor* Actor);
};
