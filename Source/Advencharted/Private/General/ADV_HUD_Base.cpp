// Fill out your copyright notice in the Description page of Project Settings.


#include "General/ADV_HUD_Base.h"

#include "Actors/ActorInspectorBase.h"
#include "General/AdvenchartedLogCategory.h"
#include "Widgets/InteractionWidgetBase.h"
#include "Widgets/MainWidgetBase.h"

void AADV_HUD_Base::BeginPlay()
{
	Super::BeginPlay();

	if (!InspectorClass)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::BeginPlay: InspectorClass is nullptr"));
		return;
	}
	Inspector = GetWorld()->SpawnActor<AActorInspectorBase>(InspectorClass);

	if (!MainWidgetClass)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::BeginPlay: MainWidgetClass is nullptr"));
		return;
	}
	MainWidget = CreateWidget<UMainWidgetBase>(PlayerOwner, MainWidgetClass);
	MainWidget->AddToViewport();	
}

UInteractionWidgetBase* AADV_HUD_Base::GetInteractionWidgetFor(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::GetInteractionWidgetFor: Actor is nullptr"));
		return nullptr;
	}

	for (const auto InteractionWidget : InteractionWidgets)
	{
		if (InteractionWidget->GetInteractableActor() == Actor)
		{
			return InteractionWidget;
		}
	}
	return nullptr;
}

void AADV_HUD_Base::InspectItem(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("AADV_HUD_Base::InspectItem: Actor is nullptr"));
		return;
	}
	
	MainWidget->InspectItem(Actor);
}
