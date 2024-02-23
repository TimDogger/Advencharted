// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActorInspectorWidgetBase.generated.h"

class UTextBlock;
class UMultiLineEditableTextBox;
class UButton;
class UImage;
class AActor;
class UTextureRenderTarget2D;
class AActorInspectorBase;

UCLASS(Abstract)
class ADVENCHARTED_API UActorInspectorWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UTextBlock> ItemNameTextBlock;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UMultiLineEditableTextBox> ItemDescriptionTextBlock;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UTextBlock> TakeTextBlock;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UButton> TakeButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<UImage> InspectorImage;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	TObjectPtr<AActorInspectorBase> Inspector;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|MainWidgetBase")
	FKey CloseKey = EKeys::Escape;

	UPROPERTY(EditDefaultsOnly, Category = "Advenchanted|MainWidgetBase")
	FKey TakeKey = EKeys::F;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	UTextureRenderTarget2D* ItemRenderTarget;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	bool bRotating = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Advenchanted|MainWidgetBase")
	FRotator CurrentRotation = FRotator::ZeroRotator;

private:
	UPROPERTY()
	TObjectPtr<AActor> InspectedActor = nullptr;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	void InitializeInspectorWidget(AActor* Actor);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|MainWidgetBase")
	void AddRotation(FRotator Delta);

	UFUNCTION(BlueprintCallable, Category = "Advenchanted|MainWidgetBase")
	void UpdateItemImage();

private:
	UFUNCTION()
	void TakeItem();
	
	UFUNCTION()
	void CloseInspector();
};
