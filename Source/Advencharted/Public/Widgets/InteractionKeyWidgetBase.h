﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "InteractionKeyWidgetBase.generated.h"


UCLASS(Abstract)
class ADVENCHARTED_API UInteractionKeyWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|InteractionKeyWidgetBase")
	TObjectPtr<class UTextBlock> InteractionKeyTextBlock;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|InteractionKeyWidgetBase")
	TObjectPtr<UTextBlock> InteractionTextBlock;

public:
	void ShowInteraction(FKey Key);
	void HideInteraction();
};
