// Fill out your copyright notice in the Description page of Project Settings.


#include "General/AdvenchartedLibrary.h"

#include "General/AdvenchartedLogCategory.h"
#include "General/ADV_HUD_Base.h"
#include "..\..\Public\Interfaces\Interactable.h"

UInteractionDefinition* UAdvenchartedLibrary::GetInteractionDefinitionForActor(UObject* WorldContextObject, AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UAdvenchartedLibrary::GetInteractionDefinitionForActor: Actor is nullptr"));
		return nullptr;
	}

	UInteractionDefinition* OutInteractionDefinition = nullptr;
	if (Actor->Implements<UInteractable>())
	{
		OutInteractionDefinition = IInteractable::Execute_GetInteractionDefinition(Actor);
	}
	if (!OutInteractionDefinition)
	{
		OutInteractionDefinition = GetDefaultInteractionDefinition(WorldContextObject);
	}
	return OutInteractionDefinition;
}

UInteractionDefinition* UAdvenchartedLibrary::GetDefaultInteractionDefinition(UObject* WorldContextObject)
{
	auto HUD = GetAdvenchantedHUD(WorldContextObject);
	if (!HUD)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UAdvenchartedLibrary::GetDefaultInteractionDefinition: HUD is nullptr"));
		return nullptr;
	}
	return HUD->DefaultInteractionDefinition;
}

AADV_HUD_Base* UAdvenchartedLibrary::GetAdvenchantedHUD(UObject* WorldContextObject)
{
	auto HUD = Cast<AADV_HUD_Base>(WorldContextObject->GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!HUD)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UAdvenchartedLibrary::GetDefaultInteractionDefinition: HUD is nullptr"));
		return nullptr;
	}
	return HUD;
}
