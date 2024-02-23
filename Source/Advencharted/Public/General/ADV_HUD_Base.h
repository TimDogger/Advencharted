// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ADV_HUD_Base.generated.h"

class UMainWidgetBase;
class UInteractionWidgetBase;
class AActorInspectorBase;
class UInteractionDefinition;

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
	TSubclassOf<AActorInspectorBase> InspectorClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "Advenchanted|HUD")
	TObjectPtr<UMainWidgetBase> MainWidget;

	UPROPERTY(BlueprintReadOnly, Category = "Advenchanted|HUD")
	TArray<TObjectPtr<UInteractionWidgetBase>> InteractionWidgets;	

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD")
	TObjectPtr<UInteractionDefinition> DefaultInteractionDefinition;

	UPROPERTY(BlueprintReadOnly, Category = "Advenchanted|HUD")
	TObjectPtr<AActorInspectorBase> Inspector;

public:	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|HUD")
	UInteractionWidgetBase* GetInteractionWidgetFor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|HUD")
	void InspectItem(AActor* Actor);
};
