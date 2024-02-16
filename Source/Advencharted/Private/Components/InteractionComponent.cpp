// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "General/AdvenchartedLogCategory.h"
#include "General/ADV_HUD_Base.h"
#include "Interfaces/Interactible.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	OnInteractionFound(OtherActor);
}

void UInteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	OnInteractionLost(OtherActor);
}

void UInteractionComponent::Interact(AActor* TargetActor)
{
	if (!TargetActor) return;
	if (TargetActor->Implements<UInteractable>())
	{
		if (const auto InteractionDefinition = IInteractable::Execute_GetInteractionDefinition(TargetActor))
		{
			InteractionDefinition->OnInteract(GetOwner(), TargetActor);
		}
		IInteractable::Execute_OnInteract(TargetActor);		
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateIntearctionSphere();
}

void UInteractionComponent::CreateIntearctionSphere()
{
	auto Owner = GetOwner();
	if (Owner)
	{
		InteractionSphere = NewObject<USphereComponent>(Owner);
		if (InteractionSphere)
		{
			InteractionSphere->RegisterComponent();
			InteractionSphere->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			InteractionSphere->SetSphereRadius(InteractionRadius);
			InteractionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
			InteractionSphere->SetVisibility(true);
			InteractionSphere->SetHiddenInGame(false);
			InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnOverlapBegin);
			InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnOverlapEnd);
			TArray<AActor*> OverlappingActors;
			InteractionSphere->GetOverlappingActors(OverlappingActors);
			for (auto OverlappingActor : OverlappingActors)
			{
				if (OverlappingActor->Implements<UInteractable>())
				{
					OnInteractionFound(OverlappingActor);
				}
			}
		}
	}
}

void UInteractionComponent::OnInteractionFound(AActor* Actor)
{
	if (!Actor) return;
	if (Actor->Implements<UInteractable>())
	{
		IInteractable::Execute_OnInteractionFound(Actor);
		
		auto HUD = GetHUD();
		if (!HUD)
		{
			UE_LOG(LogAdvencharted, Error, TEXT("UInteractionComponent::OnOverlapBegin: HUD is null"));
			return;
		}
		HUD->ShowInteractionWidgetFor(Actor);
		InteractableActors.Add(Actor);
	}
}

void UInteractionComponent::OnInteractionLost(AActor* Actor)
{
	if (!Actor) return;
	if (Actor->Implements<UInteractable>())
	{
		IInteractable::Execute_OnInteractionLost(Actor);
		
		auto HUD = GetHUD();
		if (!HUD)
		{
			UE_LOG(LogAdvencharted, Error, TEXT("UInteractionComponent::OnOverlapBegin: HUD is null"));
			return;
		}
		HUD->RemoveInteractionWidgetFor(Actor);
		InteractableActors.Remove(Actor);
	}
}

AADV_HUD_Base* UInteractionComponent::GetHUD()
{
	auto HUD = Cast<AADV_HUD_Base>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!HUD)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UInteractionComponent::OnOverlapBegin: HUD is null"));
		return nullptr;
	}
	return HUD;
}

