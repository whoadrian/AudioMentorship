// Fill out your copyright notice in the Description page of Project Settings.


#include "Mentorship_01_ScifiBase/Public/AudioAmbiencePlayerComponent.h"
#include "Mentorship_01_ScifiBase/Public/AudioAmbienceSubsystem.h"


UAudioAmbiencePlayerComponent::UAudioAmbiencePlayerComponent()
{
}

void UAudioAmbiencePlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UAudioAmbienceSubsystem>()->RegisterPlayerComponent(this);
	
}

void UAudioAmbiencePlayerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetSubsystem<UAudioAmbienceSubsystem>()->UnregisterPlayerComponent(this);
}
