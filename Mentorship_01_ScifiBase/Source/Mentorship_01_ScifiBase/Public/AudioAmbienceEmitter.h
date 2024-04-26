// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AudioAmbienceEmitter.generated.h"

class AAudioAmbienceZone;
class UAkComponent;
class UAkAudioEvent;

UCLASS()
class MENTORSHIP_01_SCIFIBASE_API AAudioAmbienceEmitter : public AActor
{
	GENERATED_BODY()

public:
	AAudioAmbienceEmitter();

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UAkAudioEvent> AmbienceEvent;

	UPROPERTY(Transient)
	TObjectPtr<UAkComponent> AkComponent;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
