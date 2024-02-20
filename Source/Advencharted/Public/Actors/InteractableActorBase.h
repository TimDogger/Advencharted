// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "InteractableActorBase.generated.h"

UCLASS(Abstract)
class ADVENCHARTED_API AInteractableActorBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractableActorBase();
};
