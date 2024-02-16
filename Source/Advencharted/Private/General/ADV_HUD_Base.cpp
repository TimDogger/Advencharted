// Fill out your copyright notice in the Description page of Project Settings.


#include "General/ADV_HUD_Base.h"
#include "General/AdvenchartedLogCategory.h"
#include "Interfaces/Interactible.h"

void AADV_HUD_Base::BeginPlay()
{
	Super::BeginPlay();

	if (!MainWidgetClass)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::BeginPlay: MainWidgetClass is nullptr"));
		return;
	}

	MainWidget = CreateWidget<UMainWidgetBase>(PlayerOwner, MainWidgetClass);
	MainWidget->AddToViewport();
}

void AADV_HUD_Base::ShowInteractionWidgetFor(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::ShowInteractionWidgetFor: Actor is nullptr"));
		return;
	}

	auto InteractionDefinition = IInteractable::Execute_GetInteractionDefinition(Actor);

	if (!InteractionDefinition)
	{
		InteractionDefinition = DefaultInteractionDefinition;
	}

	if (!InteractionDefinition->IsValid())
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::ShowInteractionWidgetFor: InteractionDefinition is not valid"));
	}
	
	const auto InteractionWidget = CreateWidget<UInteractionWidgetBase>(PlayerOwner, InteractionDefinition->InteractionWidgetClass);
	MainWidget->MainCanvas->AddChildToCanvas(InteractionWidget);
	InteractionWidget->InitializeInteractionWidget(InteractionDefinition, Actor);
	InteractionWidgets.Add(InteractionWidget);
}

void AADV_HUD_Base::RemoveInteractionWidgetFor(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::RemoveInteractionWidgetFor: Actor is nullptr"));
		return;
	}

	for (auto InteractionWidget : InteractionWidgets)
	{
		if (InteractionWidget->GetInteractableActor() == Actor)
		{
			InteractionWidget->RemoveFromParent();
			InteractionWidgets.Remove(InteractionWidget);
			break;
		}
	}
}
