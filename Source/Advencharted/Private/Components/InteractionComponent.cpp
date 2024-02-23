// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "Components/SphereComponent.h"
#include "Interfaces/Interactable.h"
#include "Kismet/KismetMathLibrary.h"
#include "General/InteractionDefinition.h"
#include "Kismet/KismetSystemLibrary.h"


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
		if (CurrentInteraction)
		{
			IInteractable::Execute_OnNotReadyToInteract(CurrentInteraction->GetOwner(), GetOwner(),
			                                            CurrentInteraction);
		}
		return;
	}

	auto CameraRotation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation();
	auto CameraForward = UKismetMathLibrary::GetForwardVector(CameraRotation);
	auto CameraLeft = UKismetMathLibrary::NegateVector(UKismetMathLibrary::GetRightVector(CameraRotation));

	auto CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	auto TraceEnd = CameraLocation + CameraForward * InteractionTraceLength + CameraLeft * CameraLeftOffset;

	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore = {GetOwner()};
	EDrawDebugTrace::Type DebugType = bDrawDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), CameraLocation, TraceEnd, InteractionTraceRadius, TraceType,
	                                       false, ActorsToIgnore, DebugType, HitResults, true);

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
		if (NewInteracion.Key == CurrentInteraction) return;
		if (CurrentInteraction)
		{
			IInteractable::Execute_OnNotReadyToInteract(CurrentInteraction->GetOwner(), GetOwner(),
			                                            CurrentInteraction);
		}
		CurrentInteraction = NewInteracion.Key;
		IInteractable::Execute_OnReadyToInteract(CurrentInteraction->GetOwner(), GetOwner(),
		                                         CurrentInteraction);
		OnCurrentInteractionUpdated.Broadcast(CurrentInteraction);
	}
	else
	{
		if (CurrentInteraction)
		{
			IInteractable::Execute_OnNotReadyToInteract(CurrentInteraction->GetOwner(), GetOwner(),
			                                            CurrentInteraction);
		}
		CurrentInteraction = nullptr;
		OnCurrentInteractionUpdated.Broadcast(CurrentInteraction);
	}
}

void UInteractionComponent::OnOverlapBegin_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                          bool bFromSweep,
                                                          const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UInteractable>())
	{
		auto InteractablePrimitive = IInteractable::Execute_GetInteractableComponent(OtherActor);
		if (!InteractablePrimitive)
		{
			auto FirstPrimitive = OtherActor->FindComponentByClass<UPrimitiveComponent>();
			if (FirstPrimitive && FirstPrimitive == OtherComp)
			{
				InteractablePrimitive = OtherComp;
			}
			else return;
		}
		IInteractable::Execute_OnInteractionFound(OtherActor, GetOwner(), InteractablePrimitive);

		InteractableComponents.Add(InteractablePrimitive);
		OnInteractionFound.Broadcast(InteractablePrimitive);
	}
}

void UInteractionComponent::OnOverlapEnd_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UInteractable>())
	{
		auto InteractablePrimitive = IInteractable::Execute_GetInteractableComponent(OtherActor);
		if (!InteractablePrimitive) InteractablePrimitive = OtherComp;
		IInteractable::Execute_OnInteractionLost(OtherActor, GetOwner(), InteractablePrimitive);
		InteractableComponents.Remove(InteractablePrimitive);
		OnInteractionLost.Broadcast(InteractablePrimitive);
	}
}

void UInteractionComponent::Interact()
{
	if (!CurrentInteraction) return;
	auto TargetActor = CurrentInteraction->GetOwner();
	if (TargetActor->Implements<UInteractable>())
	{
		if (const auto InteractionDefinition = IInteractable::Execute_GetInteractionDefinition(TargetActor))
		{
			InteractionDefinition->OnInteract(GetOwner(), TargetActor);
		}
		IInteractable::Execute_OnInteract(TargetActor, GetOwner(), CurrentInteraction);
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Do a delayed interaction sphere creation to avoid InteractionWidgets not being added to the HUD in the first frame
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInteractionComponent::CreateIntearctionSphere, 0.5f,
	                                       false);
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
