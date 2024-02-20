// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ActorInspectorWidgetBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "General/AdvenchartedLogCategory.h"
#include "General/ADV_HUD_Base.h"
#include "..\..\Public\Interfaces\Interactable.h"

void UActorInspectorWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UActorInspectorWidgetBase::CloseInspector);
	TakeButton->OnClicked.AddDynamic(this, &UActorInspectorWidgetBase::TakeItem);

	TakeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Take (%s)"), *TakeKey.ToString())));

	auto HUD = GetOwningPlayer<APlayerController>()->GetHUD<AADV_HUD_Base>();
	if (!HUD)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UActorInspectorWidgetBase::NativeConstruct: HUD is nullptr"));
		return;
	}
	Inspector = HUD->Inspector;
}

void UActorInspectorWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateItemImage();
}

FReply UActorInspectorWidgetBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == CloseKey)
	{
		CloseInspector();
		return FReply::Handled();
	}
	if (InKeyEvent.GetKey() == TakeKey)
	{
		TakeItem();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UActorInspectorWidgetBase::CloseInspector()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
	GetOwningPlayer()->SetShowMouseCursor(false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UActorInspectorWidgetBase::InitializeInspectorWidget(AActor* Actor)
{
	if (!Actor)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UActorInspectorWidgetBase::InitializeInspectorWidget: Actor is nullptr"));
		return;
	}
	InspectedActor = Actor;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this);
	GetOwningPlayer()->SetShowMouseCursor(true);
	FText Name = FText::FromString(Actor->GetName());
	FText Description = FText::FromString(TEXT("No description"));
	if (Actor->Implements<UInteractable>())
	{
		FText ItemName = IInteractable::Execute_GetItemName(Actor);
		FText ItemDescription = IInteractable::Execute_GetItemDescription(Actor);
		if (!ItemName.IsEmpty())
		{
			Name = ItemName;
		}
		if (!ItemDescription.IsEmpty())
		{
			Description = ItemDescription;
		}
	}
	ItemNameTextBlock->SetText(Name);
	ItemDescriptionTextBlock->SetText(Description);
	
	SetVisibility(ESlateVisibility::Visible);
}

FReply UActorInspectorWidgetBase::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bRotating)
	{
		auto Delta = InMouseEvent.GetCursorDelta();
		auto DeltaRotation = FRotator(Delta.Y * -1.0f, Delta.X, 0.0f);
		AddRotation(DeltaRotation);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UActorInspectorWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bRotating = true;
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UActorInspectorWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bRotating = false;
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UActorInspectorWidgetBase::AddRotation(FRotator Delta)
{
	if (!Inspector) return;
	CurrentRotation += Delta;
	CurrentRotation.Roll = 0;
	Inspector->SpringArm->SetRelativeRotation(CurrentRotation);
	UpdateItemImage();
}

void UActorInspectorWidgetBase::UpdateItemImage()
{
	FVector2d ImageSize = InspectorImage->GetBrush().ImageSize;
	if (ImageSize.X <= 0 || ImageSize.Y <= 0)
	{
		const auto Material = InspectorImage->GetDynamicMaterial();
		ImageSize = InspectorImage->GetPaintSpaceGeometry().GetLocalSize();
		const auto NewBrush = UWidgetBlueprintLibrary::MakeBrushFromMaterial(Material, ImageSize.X, ImageSize.Y);
		InspectorImage->SetBrush(NewBrush);
	}
	ItemRenderTarget = Inspector->GetActorSnapshot(InspectedActor, ImageSize.X, ImageSize.Y, ItemRenderTarget);
	auto Material = InspectorImage->GetDynamicMaterial();
	if (!Material)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UActorInspectorWidgetBase::UpdateItemImage: Material is nullptr"));
		return;
	}
	Material->SetTextureParameterValue("Texture", ItemRenderTarget);
}

void UActorInspectorWidgetBase::TakeItem()
{
	if (!InspectedActor)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UActorInspectorWidgetBase::TakeItem: InspectedActor is nullptr"));
		CloseInspector();
	}
	InspectedActor->Destroy();
	CloseInspector();
}
