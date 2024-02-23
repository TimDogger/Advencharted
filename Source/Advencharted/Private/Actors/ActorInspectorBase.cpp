// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ActorInspectorBase.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetRenderingLibrary.h"


AActorInspectorBase::AActorInspectorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(SpringArm);
	SceneCapture->FOVAngle = DevaultFov;
	SceneCapture->SetComponentTickEnabled(true);
	SceneCapture->ProjectionType = ECameraProjectionMode::Perspective;
	SceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	SceneCapture->CaptureSource = SCS_FinalColorLDR;
	SceneCapture->bCaptureEveryFrame = true;
	SceneCapture->bCaptureOnMovement = false;
}

void AActorInspectorBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ItemRenderTarget)
	{
		ItemRenderTarget->ReleaseResource();
	}
}

UTextureRenderTarget2D* AActorInspectorBase::GetActorSnapshot(AActor* Actor, int32 ResolutionX, int32 ResolutionY,
                                                                          UTextureRenderTarget2D* RenderTarget)
{
	if (!Actor)
	{
		return nullptr;
	}
	
	if (!RenderTarget)
	{
		ETextureRenderTargetFormat RenderTargetFormat = RTF_RGBA16f;
		FColor ClearColor = FColor::Black;
		bool bAutoGenerateMipMaps = true;
		bool bCanCreateUAV = false;
		ItemRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(this, ResolutionX, ResolutionY,
		                                                                 RenderTargetFormat, ClearColor,
		                                                                 bAutoGenerateMipMaps, bCanCreateUAV);
		SceneCapture->TextureTarget = ItemRenderTarget;
	}
	else if (RenderTarget != ItemRenderTarget)
	{
		ItemRenderTarget->ReleaseResource();
		ItemRenderTarget = RenderTarget;
		SceneCapture->TextureTarget = ItemRenderTarget;
	}

	PreCaptureSetup(Actor);
	
	bool bIsHidden = ItemActor->IsHidden();

	SetupPrimitives(true);
	SceneCapture->CaptureScene();
	ItemActor->SetHidden(bIsHidden);
	SetupPrimitives(false);

	return ItemRenderTarget;
}

void AActorInspectorBase::ResetCamera_Implementation()
{
	auto ParentActor = GetAttachParentActor();
	if (ParentActor)
	{
		SetActorTransform(ParentActor->GetTransform());
		FVector NewLocation = FVector::Zero();
		SceneCapture->FOVAngle = DevaultFov;
		SpringArm->SetRelativeLocationAndRotation(NewLocation, DefaultRotation);
	}
}

void AActorInspectorBase::RotateCamera_Implementation(FRotator Delta)
{
	SpringArm->AddRelativeRotation(Delta);
	if (ItemRenderTarget)
	{
		SceneCapture->CaptureScene();
	}
}

void AActorInspectorBase::SetCameraDistance_Implementation(float NewDistance)
{
	SpringArm->TargetArmLength = NewDistance;
}

float AActorInspectorBase::GetDistance() const
{
	if (ItemActor)
	{
		FVector Origin, Extent;
		ItemActor->GetActorBounds(false, Origin, Extent);
		return Extent.Size() * CameraDistanceBounceRatio;
	}
	return 0.0f;
}

void AActorInspectorBase::PreCaptureSetup(AActor* NewActor)
{
	if (ItemActor == NewActor) return;
	ItemActor = NewActor;
	AttachToActor(ItemActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PrimitiveComponents.Empty();
	ResetCamera();
	SceneCapture->ClearShowOnlyComponents();
	SceneCapture->ShowOnlyActorComponents(ItemActor, true);
	TArray<AActor*> ChildActors;
	ItemActor->GetAllChildActors(ChildActors);
	for (auto ChildActor : ChildActors)
	{
		SceneCapture->ShowOnlyActorComponents(ChildActor, false);
	}
	
	bool bIsCollisionEnabled = ItemActor->GetActorEnableCollision();
	ItemActor->SetActorEnableCollision(true);
	auto Rotation = ItemActor->GetActorRotation();
	ItemActor->SetActorRotation(FRotator::ZeroRotator);

	TArray<ECollisionEnabled::Type> CollisionsEnabled;
	for (auto Component : ItemActor->GetComponentsByInterface(UPhysicsComponent::StaticClass()))
	{
		if (auto PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
		{
			if (Cast<UArrowComponent>(PrimitiveComponent)) continue;
			PrimitiveComponents.Add(PrimitiveComponent);
			PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			CollisionsEnabled.Add(PrimitiveComponent->GetCollisionEnabled());
		}
	}

	FVector Origin;
	FVector BoxExtent;
	ItemActor->GetActorBounds(true, Origin, BoxExtent, true);
	float Extent = BoxExtent.Length();

	ItemActor->SetActorRotation(Rotation);
	for (int32 i = 0; i < PrimitiveComponents.Num(); i++)
	{
		if (Cast<UArrowComponent>(PrimitiveComponents[i])) continue;
		PrimitiveComponents[i]->SetCollisionEnabled(CollisionsEnabled[i]);
	}
	ItemActor->SetActorEnableCollision(bIsCollisionEnabled);

	FVector RelativeOffset = Origin - ItemActor->GetActorLocation();
	SetActorRelativeLocation(RelativeOffset);
	Extent *= CameraDistanceBounceRatio;
	SpringArm->TargetArmLength = Extent;
	SceneCapture->OrthoWidth = Extent;

	ItemActor->SetActorHiddenInGame(false);
}

void AActorInspectorBase::SetupPrimitives(bool bForCapture)
{
	for (auto PrimitiveComponent : PrimitiveComponents)
	{
		PrimitiveComponent->SetLightingChannels(!bForCapture, bForCapture, false);
		PrimitiveComponent->SetCastShadow(!bForCapture);
	}
}