// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "ActorInspectorWidgetBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ADVENCHARTED_API UActorInspectorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class UTextBlock> ItemNameTextBlock;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class UMultiLineEditableTextBox> ItemDescriptionTextBlock;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UTextBlock> TakeTextBlock;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class UButton> CloseButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UButton> TakeButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class UImage> InspectorImage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<class AActorInspectorBase> Inspector;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|MainWidgetBase")
	FKey CloseKey = EKeys::Escape;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|MainWidgetBase")
	FKey TakeKey = EKeys::F;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	UTextureRenderTarget2D* ItemRenderTarget;

private:
	UPROPERTY()
	TObjectPtr<AActor> InspectedActor = nullptr;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void InitializeInspectorWidget(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|MainWidgetBase")
	void UpdateItemImage();

private:
	UFUNCTION()
	void TakeItem();
	
	UFUNCTION()
	void CloseInspector();
};
