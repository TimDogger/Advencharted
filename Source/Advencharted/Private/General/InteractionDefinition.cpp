// Fill out your copyright notice in the Description page of Project Settings.


#include "General/InteractionDefinition.h"

#include "Widgets/InteractionWidgetBase.h"

bool UInteractionDefinition::IsValid()
{
	return (!InteractionText.IsEmpty() && InteractionWidgetClass);
}
