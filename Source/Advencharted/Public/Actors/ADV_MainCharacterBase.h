// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ADV_MainCharacterBase.generated.h"

class UInteractionComponent;

UCLASS()
class ADVENCHARTED_API AADV_MainCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainCharacterBase")
	TObjectPtr<UInteractionComponent> InteractionComponent;

public:
	AADV_MainCharacterBase();
};
