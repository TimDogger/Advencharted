// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainWidgetBase.h"

#include "Actors/ADV_MainCharacterBase.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/InteractionComponent.h"
#include "Components/TextBlock.h"
#include "General/AdvenchartedLibrary.h"
#include "General/AdvenchartedLogCategory.h"
#include "General/InteractionDefinition.h"
#include "Interfaces/Interactable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Widgets/ActorInspectorWidgetBase.h"
#include "Widgets/InteractionKeyWidgetBase.h"

UMainWidgetBase::UMainWidgetBase(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UMainWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto PlayerController = GetOwningPlayer();
	MainCharacter = Cast<AADV_MainCharacterBase>(PlayerController->GetPawn());
	if (!MainCharacter)
	{
		UE_LOG(LogAdvencharted, Warning, TEXT("UMainWidgetBase::NativeOnInitialized: MainCharacter is nullptr"));
	}
	MainCharacter->InteractionComponent->OnInteractionFound.
	               AddDynamic(this, &UMainWidgetBase::ShowInteractionWidgetFor);
	MainCharacter->InteractionComponent->OnInteractionLost.AddDynamic(
		this, &UMainWidgetBase::RemoveInteractionWidgetFor);
	MainCharacter->InteractionComponent->OnCurrentInteractionUpdated.AddDynamic(
		this, &UMainWidgetBase::OnCurrentInteractionUpdated);
	OnCurrentInteractionUpdated(nullptr);

	InspectorWidget->OnVisibilityChanged.AddDynamic(this, &UMainWidgetBase::OnInspectorVisibilityChanged);
	CameraManager = PlayerController->PlayerCameraManager;
	ActorsToIgnore.SetNum(2);
}

void UMainWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (InspectorWidget->IsVisible()) return;
	for (const auto InteractionWidget : InteractionWidgets)
	{
		const auto Widget = InteractionWidget.Value;
		FVector2d Position = Widget->GetCanvasPanelSlot()->GetPosition();
		bool bWorldLocation = false;
		FVector InteractableLocation = IInteractable::Execute_GetInteractableLocation(
			InteractionWidget.Key, bWorldLocation);
		FVector WorldLocation = bWorldLocation
			                        ? InteractableLocation
			                        : InteractionWidget.Key->GetActorLocation() + InteractableLocation;

		FHitResult HitResult;
		ActorsToIgnore.Empty();
		ActorsToIgnore.Add(MainCharacter);
		ActorsToIgnore.Add(InteractionWidget.Key);
		UKismetSystemLibrary::LineTraceSingle(this, CameraManager->GetCameraLocation(), WorldLocation,
		                                      UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
		                                      EDrawDebugTrace::None, HitResult, true);
		if (HitResult.bBlockingHit)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
			continue;
		}

		if (UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), WorldLocation, Position,
		                                                               false))
		{
			Widget->GetCanvasPanelSlot()->SetPosition(Position);
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UMainWidgetBase::OnInspectorVisibilityChanged(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		for (auto InteractionWidget : InteractionWidgets)
		{
			InteractionWidget.Value->SetVisibility(ESlateVisibility::Collapsed);
		}
		InteractionKeyWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		for (auto InteractionWidget : InteractionWidgets)
		{
			InteractionWidget.Value->SetVisibility(ESlateVisibility::Visible);
		}
		if (MainCharacter->InteractionComponent->GetCurrentInteraction())
		{
			InteractionKeyWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainWidgetBase::InspectItem(AActor* Actor)
{
	InspectorWidget->InitializeInspectorWidget(Actor);
}

void UMainWidgetBase::ShowInteractionWidgetFor(UPrimitiveComponent* InteractionComponent)
{
	if (!InteractionComponent)
	{
		UE_LOG(LogAdvencharted, Warning,
		       TEXT("UMainWidgetBase::ShowInteractionWwidgetFor: InteractionComponent is nullptr"));
		return;
	}

	auto Actor = InteractionComponent->GetOwner();
	const auto InteractionDefinition = UAdvenchartedLibrary::GetInteractionDefinitionForActor(this, Actor);
	if (!InteractionDefinition)
	{
		UE_LOG(LogAdvencharted, Warning,
		       TEXT("UMainWidgetBase::ShowInteractionWidgetFor: InteractionDefinition is nullptr"));
		return;
	}

	auto InteractionWidget = CreateWidget<UInteractionWidgetBase>(GetOwningPlayer(),
	                                                              InteractionDefinition->InteractionWidgetClass);
	MainCanvas->AddChildToCanvas(InteractionWidget);
	InteractionWidget->InitializeInteractionWidget(Actor);
	InteractionWidgets.Add(Actor, InteractionWidget);
}

void UMainWidgetBase::RemoveInteractionWidgetFor(UPrimitiveComponent* InteractionComponent)
{
	if (!InteractionComponent)
	{
		UE_LOG(LogAdvencharted, Warning,
		       TEXT("UMainWidgetBase::RemoveInteractionWidgetFor: InteractionComponent is nullptr"));
		return;
	}

	auto Actor = InteractionComponent->GetOwner();
	if (auto InteractionWidget = InteractionWidgets.FindRef(Actor))
	{
		InteractionWidget->RemoveFromParent();
		InteractionWidgets.Remove(Actor);
	}
}

void UMainWidgetBase::OnCurrentInteractionUpdated(UPrimitiveComponent* HitComponent)
{
	if (!HitComponent)
	{
		InteractionKeyWidget->InteractionTextBlock->SetText(FText::FromString(""));
		InteractionKeyWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	const auto InteractionDefinition = UAdvenchartedLibrary::GetInteractionDefinitionForActor(
		this, HitComponent->GetOwner());
	if (!InteractionDefinition)
	{
		UE_LOG(LogAdvencharted, Warning,
		       TEXT("UMainWidgetBase::OnCurrentInteractionUpdated: InteractionDefinition is nullptr"));
		return;
	}
	InteractionKeyWidget->InteractionTextBlock->SetText(InteractionDefinition->InteractionText);
	InteractionKeyWidget->SetVisibility(ESlateVisibility::Visible);
}
