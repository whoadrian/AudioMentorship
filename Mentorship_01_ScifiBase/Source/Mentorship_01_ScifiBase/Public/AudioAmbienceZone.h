// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "AudioAmbienceZone.generated.h"

struct FGameplayTag;
class UAkAudioEvent;

UCLASS()
class MENTORSHIP_01_SCIFIBASE_API AAudioAmbienceZone : public AActor
{
	GENERATED_BODY()

public:
	AAudioAmbienceZone();

	UPROPERTY(EditInstanceOnly)
	FGameplayTag ZoneTag;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
