// Fill out your copyright notice in the Description page of Project Settings.


#include "Mentorship_01_ScifiBase/Public/AudioAmbienceZone.h"

#include "Mentorship_01_ScifiBase/Public/AudioAmbienceSubsystem.h"


AAudioAmbienceZone::AAudioAmbienceZone()
{
}

void AAudioAmbienceZone::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetSubsystem<UAudioAmbienceSubsystem>()->RegisterAmbienceZone(this);
}

void AAudioAmbienceZone::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetSubsystem<UAudioAmbienceSubsystem>()->UnregisterAmbienceZone(this);
}
