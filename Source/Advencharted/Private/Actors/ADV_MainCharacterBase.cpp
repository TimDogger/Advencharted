// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ADV_MainCharacterBase.h"

#include "Components/InteractionComponent.h"


AADV_MainCharacterBase::AADV_MainCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
}

