// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteractionWidgetBase.h"

#include "Components/CanvasPanelSlot.h"
#include "General/AdvenchartedLogCategory.h"

UInteractionWidgetBase::UInteractionWidgetBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

UCanvasPanelSlot* UInteractionWidgetBase::GetCanvasPanelSlot() const
{
	return CanvasPanelSlot;
}

AActor* UInteractionWidgetBase::GetInteractableActor()
{
	return InteractableActor;
}

void UInteractionWidgetBase::InitializeInteractionWidget_Implementation(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UInteractionWidgetBase::InitializeInteractionWidget: Actor is null"));
		return;
	}
	CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	CanvasPanelSlot->SetAutoSize(true);
	CanvasPanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	InteractableActor = Actor;
}
