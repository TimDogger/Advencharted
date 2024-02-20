// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AdvenchartedLibrary.generated.h"

class UInteractionDefinition;

UCLASS()
class ADVENCHARTED_API UAdvenchartedLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|Library", meta = (WorldContext = "WorldContextObject"))
	static UInteractionDefinition* GetInteractionDefinitionForActor(UObject* WorldContextObject, AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|Library", meta = (WorldContext = "WorldContextObject"))
	static UInteractionDefinition* GetDefaultInteractionDefinition(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Advenchanted|Library", meta = (WorldContext = "WorldContextObject"))
	static class AADV_HUD_Base* GetAdvenchantedHUD(UObject* WorldContextObject);
};
