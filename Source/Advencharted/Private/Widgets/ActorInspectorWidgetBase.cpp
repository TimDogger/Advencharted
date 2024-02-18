// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ActorInspectorWidgetBase.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Engine/TextureRenderTarget2D.h"
#include "General/AdvenchartedLogCategory.h"
#include "General/ADV_HUD_Base.h"

void UActorInspectorWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	CloseButton->OnClicked.AddDynamic(this, &UActorInspectorWidgetBase::CloseInspector);
	TakeButton->OnClicked.AddDynamic(this, &UActorInspectorWidgetBase::TakeItem);

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
	SetVisibility(ESlateVisibility::Visible);
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
