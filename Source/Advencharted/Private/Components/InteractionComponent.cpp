// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "General/AdvenchartedLibrary.h"
#include "General/AdvenchartedLogCategory.h"
#include "General/ADV_HUD_Base.h"
#include "Interfaces/Interactible.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateInteraction();
}

void UInteractionComponent::UpdateInteraction()
{
	if (InteractableComponents.IsEmpty())
	{
		if (CurrentInteraction.Key)
		{
			IInteractable::Execute_OnNotReadyToInteract(CurrentInteraction.Key->GetOwner(), GetOwner(),
															CurrentInteraction.Key);
		}
		return;
	}
	
	auto CameraForward = UKismetMathLibrary::GetForwardVector(
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation());

	auto CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), CameraLocation,
	                                       CameraLocation + CameraForward * InteractionTraceLength,
	                                       InteractionTraceRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility),
	                                       false, {}, EDrawDebugTrace::None, HitResults, true);

	TTuple<UPrimitiveComponent*, float> NewInteracion = TTuple<UPrimitiveComponent*, float>(nullptr, -1.0f);
	for (const auto& HitResult : HitResults)
	{
		auto PrimitiveComponent = HitResult.GetComponent();
		if (PrimitiveComponent && InteractableComponents.Contains(PrimitiveComponent))
		{
			FVector DirToActor = (PrimitiveComponent->GetOwner()->GetActorLocation() - GetOwner()->GetActorLocation()).
				GetSafeNormal();
			float DotProduct = FVector::DotProduct(CameraForward, DirToActor);
			if (DotProduct > NewInteracion.Value)
			{
				NewInteracion.Key = PrimitiveComponent;
				NewInteracion.Value = DotProduct;
			}
		}
	}

	if (NewInteracion.Key)
	{
		if (NewInteracion.Key == CurrentInteraction.Key) return;
		if (CurrentInteraction.Key)
		{
			if (auto Widget = HUD->GetInteractionWidgetFor(CurrentInteraction.Key->GetOwner()))
			{
				Widget->OnNotReadyToInteract(GetOwner(), CurrentInteraction.Key);
			}
			IInteractable::Execute_OnNotReadyToInteract(CurrentInteraction.Key->GetOwner(), GetOwner(),
			                                                CurrentInteraction.Key);
		}
		CurrentInteraction = NewInteracion;
		if (auto Widget = HUD->GetInteractionWidgetFor(CurrentInteraction.Key->GetOwner()))
		{
			Widget->OnReadyToInteract(GetOwner(), CurrentInteraction.Key);
		}
		IInteractable::Execute_OnReadyToInteract(CurrentInteraction.Key->GetOwner(), GetOwner(),
		                                             CurrentInteraction.Key);
		UE_LOG(LogAdvencharted, Log,
		       TEXT("UInteractionComponent::UpdateInteraction: NewInteracion: %s  DotProduct: %f"),
		       *NewInteracion.Key->GetOwner()->GetName(), NewInteracion.Value);
	}
	else
	{
		if (CurrentInteraction.Key)
		{
			if (auto Widget = HUD->GetInteractionWidgetFor(CurrentInteraction.Key->GetOwner()))
			{
				Widget->OnNotReadyToInteract(GetOwner(), CurrentInteraction.Key);
			}
			IInteractable::Execute_OnNotReadyToInteract(CurrentInteraction.Key->GetOwner(), GetOwner(),
			                                                CurrentInteraction.Key);
		}
		CurrentInteraction = TTuple<UPrimitiveComponent*, float>(nullptr, -1.0f);
	
	}
}

void UInteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	OnInteractionFound(OtherActor, OtherComp);
}

void UInteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	OnInteractionLost(OtherActor, OtherComp);
}

void UInteractionComponent::Interact()
{
	if (!CurrentInteraction.Key) return;
	auto TargetActor = CurrentInteraction.Key->GetOwner();
	if (TargetActor->Implements<UInteractable>())
	{
		if (const auto InteractionDefinition = IInteractable::Execute_GetInteractionDefinition(TargetActor))
		{
			InteractionDefinition->OnInteract(GetOwner(), TargetActor);
		}
		if (auto Widget = HUD->GetInteractionWidgetFor(TargetActor))
		{
			Widget->OnInteract(GetOwner(), CurrentInteraction.Key);
		}
		IInteractable::Execute_OnInteract(TargetActor, GetOwner(), CurrentInteraction.Key);
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	HUD = Cast<AADV_HUD_Base>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!HUD)
	{
		UE_LOG(LogAdvencharted, Error, TEXT("UIntearctionComponent::BeginPlay: HUD is nullptr"));
	}
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
			InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnOverlapBegin);
			InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnOverlapEnd);
			InteractionSphere->RegisterComponent();
			InteractionSphere->AttachToComponent(Owner->GetRootComponent(),
			                                     FAttachmentTransformRules::KeepRelativeTransform);
			InteractionSphere->SetSphereRadius(InteractionRadius);
			InteractionSphere->SetCollisionProfileName(TEXT("InteractionTrigger"));
		}
	}
}

void UInteractionComponent::OnInteractionFound(AActor* Actor, UPrimitiveComponent* HitComponent)
{
	if (!Actor) return;
	if (Actor->Implements<UInteractable>())
	{
		auto InteractablePrimitive = IInteractable::Execute_GetInteractableComponent(Actor);
		if (!InteractablePrimitive)
		{
			auto FirstPrimitive = Actor->FindComponentByClass<UPrimitiveComponent>();
			if (FirstPrimitive && FirstPrimitive == HitComponent)
			{
				InteractablePrimitive = HitComponent;
			}
			else return;
		}
		IInteractable::Execute_OnInteractionFound(Actor, GetOwner(), InteractablePrimitive);

		HUD->ShowInteractionWidgetFor(Actor);
		auto InteractionWidget = HUD->GetInteractionWidgetFor(Actor);
		if (InteractionWidget && InteractionWidget->Implements<UInteractable>())
		{
			IInteractable::Execute_OnInteractionFound(InteractionWidget, GetOwner(), InteractablePrimitive);
		}
		InteractableComponents.Add(InteractablePrimitive);
	}
}

void UInteractionComponent::OnInteractionLost(AActor* Actor, UPrimitiveComponent* HitComponent)
{
	if (!Actor) return;
	if (Actor->Implements<UInteractable>())
	{
		auto InteractablePrimitive = IInteractable::Execute_GetInteractableComponent(Actor);
		if (!InteractablePrimitive) InteractablePrimitive = HitComponent;
		IInteractable::Execute_OnInteractionLost(Actor, GetOwner(), InteractablePrimitive);
		
		auto InteractionWidget = HUD->GetInteractionWidgetFor(Actor);
		if (InteractionWidget && InteractionWidget->Implements<UInteractable>())
		{
			IInteractable::Execute_OnInteractionLost(InteractionWidget, GetOwner(), InteractablePrimitive);
		}
		HUD->RemoveInteractionWidgetFor(Actor);
		InteractableComponents.Remove(InteractablePrimitive);
	}
}
