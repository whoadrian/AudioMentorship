// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "AudioAmbienceSubsystem.generated.h"

class UAudioAmbiencePlayerComponent;
class AAudioAmbienceEmitter;
class AAudioAmbienceZone;
class UAkAudioEvent;

USTRUCT()
struct FAudioAmbienceSystemZoneData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UAkAudioEvent> AmbienceEvent;
};

UCLASS()
class UAudioAmbienceSystemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly)
	TMap<FGameplayTag, FAudioAmbienceSystemZoneData> Zones;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};

USTRUCT()
struct FAudioSystemInstancedZoneData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AAudioAmbienceEmitter> Emitter;

	UPROPERTY()
	TArray<TObjectPtr<AAudioAmbienceZone>> Zones;
};

/**
 * 
 */
UCLASS()
class MENTORSHIP_01_SCIFIBASE_API UAudioAmbienceSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	void RegisterAmbienceZone(AAudioAmbienceZone* InZone);
	void UnregisterAmbienceZone(AAudioAmbienceZone* InZone);

	void RegisterPlayerComponent(UAudioAmbiencePlayerComponent* InPlayerComponent);
	void UnregisterPlayerComponent(UAudioAmbiencePlayerComponent* InPlayerComponent);

	void DebugDraw(float DeltaTime);

	UPROPERTY(Transient)
	TObjectPtr<UAudioAmbienceSystemData> Data;

	UPROPERTY(Transient)
	TMap<FGameplayTag, FAudioSystemInstancedZoneData> Zones;

	UPROPERTY(Transient)
	TObjectPtr<UAudioAmbiencePlayerComponent> PlayerComponent;
};
