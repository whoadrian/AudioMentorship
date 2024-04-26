// Fill out your copyright notice in the Description page of Project Settings.


#include "Mentorship_01_ScifiBase/Public/AudioAmbienceEmitter.h"

#include "AkComponent.h"

AAudioAmbienceEmitter::AAudioAmbienceEmitter()
{
	AkComponent = CreateDefaultSubobject<UAkComponent>("AudioComponent");
	AkComponent->StopWhenOwnerDestroyed = true;
}

void AAudioAmbienceEmitter::BeginPlay()
{
	Super::BeginPlay();
}

void AAudioAmbienceEmitter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

