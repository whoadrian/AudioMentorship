// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AudioAmbienceZone.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "AudioAmbiencePlayerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MENTORSHIP_01_SCIFIBASE_API UAudioAmbiencePlayerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAudioAmbiencePlayerComponent();

	UPROPERTY(EditAnywhere, Category="Ambience Rays")
	bool bAmbienceRayEnabled = true;	
	
	UPROPERTY(EditAnywhere, Category="Ambience Rays")
	FGameplayTag AmbienceRayZoneTag;

	UPROPERTY(EditAnywhere, Category="Ambience Rays")
	int AmbienceRayCount = 8;

	UPROPERTY(EditAnywhere, Category="Ambience Rays")
	float AmbienceRayMaxDistance = 100.0f;	

	UPROPERTY(EditAnywhere, Category="Ambience Rays")
	float AmbienceRayMinDistance = 1.0f;

	UPROPERTY(EditAnywhere, Category="Ambience Rays")
	FVector AmbienceRaySpawnOffset = {0, 0, 50};
	
protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AAudioAmbienceZone>> AmbienceRayZones;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
