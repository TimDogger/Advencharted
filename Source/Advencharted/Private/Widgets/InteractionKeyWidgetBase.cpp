// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteractionKeyWidgetBase.h"

#include "Components/TextBlock.h"

void UInteractionKeyWidgetBase::ShowInteraction(FKey Key)
{
	InteractionKeyTextBlock->SetText(FText::FromName(Key.GetFName()));
	SetVisibility(ESlateVisibility::Visible);
}

void UInteractionKeyWidgetBase::HideInteraction()
{
	InteractionKeyTextBlock->SetText(FText::FromString(""));
	SetVisibility(ESlateVisibility::Hidden);
}
