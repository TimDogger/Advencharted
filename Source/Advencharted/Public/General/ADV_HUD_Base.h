// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widgets/InteractionWidgetBase.h"
#include "Widgets/MainWidgetBase.h"
#include "ADV_HUD_Base.generated.h"

UCLASS(Abstract)
class ADVENCHARTED_API AADV_HUD_Base : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|HUD")
	TObjectPtr<UMainWidgetBase> MainWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD")
	TSubclassOf<UMainWidgetBase> MainWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD")
	TSubclassOf<UInteractionWidgetBase> InteractionWidgetClass;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|HUD", Instanced)
	TArray<TObjectPtr<UInteractionWidgetBase>> InteractionWidgets;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|HUD", Instanced)
	TObjectPtr<UInteractionDefinition> DefaultInteractionDefinition;

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Advenchanted|HUD")
	void ShowInteractionWidgetFor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|HUD")
	void RemoveInteractionWidgetFor(AActor* Actor);
};
