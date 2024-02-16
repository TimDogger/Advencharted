// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InteractionWidgetBase.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "General/AdvenchartedLogCategory.h"

UInteractionWidgetBase::UInteractionWidgetBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UInteractionWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdatePosition();
}

AActor* UInteractionWidgetBase::GetInteractableActor() const
{
	return InteractableActor;
}

void UInteractionWidgetBase::UpdatePosition()
{
	if (!InteractableActor || !CanvasPanelSlot)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UInteractionWidgetBase::UpdatePosition: InteractableActor or CanvasPanelSlot is null"));
		return;
	}

	FVector2d Position = CanvasPanelSlot->GetPosition();
	if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), InteractableActor->GetActorLocation(), Position, false))
	{
		if (Visibility != ESlateVisibility::Visible)
		{
			SetVisibility(ESlateVisibility::Visible);
		}
		CanvasPanelSlot->SetPosition(Position);
	}
	else
	{
		if (Visibility != ESlateVisibility::Hidden)
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}	
}

void UInteractionWidgetBase::InitializeInteractionWidget_Implementation(const UInteractionDefinition* InteractionDefinition, AActor* Actor)
{
	if (!InteractionDefinition)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UInteractionWidgetBase::InitializeInteractionWidget: InteractionDefinition is null"));
		return;
	}

	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UInteractionWidgetBase::InitializeInteractionWidget: Actor is null"));
		return;
	}
	InteractableActor = Actor;

	if (!InteractionDefinition->InteractionIcon || InteractionDefinition->InteractionText.IsEmpty())
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UInteractionWidgetBase::InitializeInteractionWidget: InteractionDefinition is missing icon or text"));
		return;
	}

	InteractionImage->SetBrushFromTexture(InteractionDefinition->InteractionIcon);
	InteractionTextBlock->SetText(InteractionDefinition->InteractionText);
	CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	UpdatePosition();
}
