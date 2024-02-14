// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ADV_MainCharacterBase.h"


// Sets default values
AADV_MainCharacterBase::AADV_MainCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AADV_MainCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AADV_MainCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AADV_MainCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

