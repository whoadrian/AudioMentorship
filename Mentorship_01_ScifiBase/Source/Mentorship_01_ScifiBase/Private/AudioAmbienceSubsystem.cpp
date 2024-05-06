// Fill out your copyright notice in the Description page of Project Settings.


#include "Mentorship_01_ScifiBase/Public/AudioAmbienceSubsystem.h"

#include "Engine/AssetManager.h"
#include "Mentorship_01_ScifiBase/Public/AudioAmbienceZone.h"
#include "AkComponent.h"
#include "AkAudioEvent.h"
#include "Mentorship_01_ScifiBase/Public/AudioAmbienceEmitter.h"

FPrimaryAssetId UAudioAmbienceSystemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(FPrimaryAssetType("AudioAmbienceSystemData"), GetFName());
}

void UAudioAmbienceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TArray<FAssetData> Assets;
	UAssetManager::Get().GetPrimaryAssetDataList(FPrimaryAssetType(FName("AudioAmbienceSystemData")), Assets);
	if(Assets.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Ambience Data not found!"))
		return;
	}
	
	Data = Cast<UAudioAmbienceSystemData>(Assets[0].GetAsset());
	if (!IsValid(Data))
	{
		UE_LOG(LogTemp, Error, TEXT("Ambience Data not found!"))
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Ambience System Initialized!"));
}

void UAudioAmbienceSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

void UAudioAmbienceSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FAkAudioDevice* AkDevice = FAkAudioDevice::Get();
	if (!AkDevice)
	{
		return;
	}

	for (auto& ZoneData : Zones)
	{
		TArray<FTransform> Positions;
		for (auto& Zone : ZoneData.Value.Zones)
		{
			Positions.Add(Zone->GetActorTransform());
		}
	
		AkDevice->SetMultiplePositions(ZoneData.Value.Emitter->AkComponent, Positions, AkMultiPositionType::MultiDirections);

		// TODO : Obstruction & Occlusion
		//AkDevice->SetMultipleObstructionAndOcclusion()
	}

	DebugDraw(DeltaTime);
}

TStatId UAudioAmbienceSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UAudioAmbienceSubsystem, STATGROUP_Tickables);
}

void UAudioAmbienceSubsystem::RegisterAmbienceZone(AAudioAmbienceZone* InZone)
{
	if (!Data->Zones.Contains(InZone->ZoneTag))
	{
		UE_LOG(LogTemp, Error, TEXT("Ambience Zone Tag Name %s doesn't exist in the subsystem data"), *InZone->ZoneTag.GetTagName().ToString())
		return;
	}

	auto ZoneEvent = Data->Zones[InZone->ZoneTag].AmbienceEvent;
	if (!IsValid(ZoneEvent))
	{
		UE_LOG(LogTemp, Error, TEXT("Ambience Zone Tag Name %s doesn't have a valid event in the subsystem data"), *InZone->ZoneTag.GetTagName().ToString());
		return;
	}

	if (!Zones.Contains(InZone->ZoneTag))
	{
		Zones.Add(InZone->ZoneTag);

		// Initialize zone
		Zones[InZone->ZoneTag].Emitter = GetWorld()->SpawnActor<AAudioAmbienceEmitter>(FVector::ZeroVector, FRotator::ZeroRotator);
		Zones[InZone->ZoneTag].Emitter->AkComponent->OcclusionRefreshInterval = 0.0f; // Disable occlusion for now
		Zones[InZone->ZoneTag].Emitter->AkComponent->PostAkEvent(ZoneEvent);

		UE_LOG(LogTemp, Warning, TEXT("Ambience Zone initialized for tag %s"), *InZone->ZoneTag.GetTagName().ToString());
	}

	Zones[InZone->ZoneTag].Zones.Add(InZone);
}

void UAudioAmbienceSubsystem::UnregisterAmbienceZone(AAudioAmbienceZone* InZone)
{
	if (!Zones.Contains(InZone->ZoneTag))
	{
		return;
	}

	Zones[InZone->ZoneTag].Zones.Remove(InZone);

	if (Zones[InZone->ZoneTag].Zones.Num() == 0)
	{
		// De-initialize zone
		GetWorld()->DestroyActor(Zones[InZone->ZoneTag].Emitter);
		
		Zones.Remove(InZone->ZoneTag);

		UE_LOG(LogTemp, Warning, TEXT("Ambience Zone DE-initialized for tag %s"), *InZone->ZoneTag.GetTagName().ToString());
	}
}

void UAudioAmbienceSubsystem::RegisterPlayerComponent(UAudioAmbiencePlayerComponent* InPlayerComponent)
{
	PlayerComponent = InPlayerComponent;
}

void UAudioAmbienceSubsystem::UnregisterPlayerComponent(UAudioAmbiencePlayerComponent* InPlayerComponent)
{
	if (PlayerComponent == InPlayerComponent)
	{
		PlayerComponent = nullptr;
	}
}

void UAudioAmbienceSubsystem::DebugDraw(float DeltaTime)
{
	FString OnScreenDebugString = FString::Printf(TEXT("Ambience System\n"));
	OnScreenDebugString += FString::Printf(TEXT("---------------------------\n"));
	
	for (auto& Entry : Zones)
	{
		auto& ZoneTag = Entry.Key;
		auto& ZoneData = Entry.Value;

		OnScreenDebugString += FString::Printf(TEXT("Zone Tag : %s\n"), *ZoneTag.GetTagName().ToString());
		OnScreenDebugString += FString::Printf(TEXT("Zone Event : %s\n"), *Data->Zones[ZoneTag].AmbienceEvent.GetName());
		OnScreenDebugString += FString::Printf(TEXT("Zones Num : %d\n"), ZoneData.Zones.Num());
		OnScreenDebugString += FString::Printf(TEXT("---------------------------\n"));

		for (auto& Zone : ZoneData.Zones)
		{
			FString DebugString = FString::Printf(TEXT("%s\n"), *Zone->GetActorNameOrLabel());
			DebugString += FString::Printf(TEXT("Tag: %s\n"), *ZoneTag.GetTagName().ToString());
			DebugString += FString::Printf(TEXT("Event: %s\n"), *Data->Zones[ZoneTag].AmbienceEvent.GetName());

			DrawDebugSphere(GetWorld(), Zone->GetActorLocation(), 50, 32,
				FColor::Green, false, DeltaTime, 1);

			if (Data->bDrawAttenuation)
			{
				DrawDebugSphere(GetWorld(), Zone->GetActorLocation(), Data->Zones[ZoneTag].AmbienceEvent->MaxAttenuationRadius, 32,
				FColor::Blue, false, DeltaTime, 1);
			}
	
			DrawDebugString(GetWorld(), Zone->GetActorLocation() + FVector::DownVector * 60,
				DebugString, nullptr, FColor::Green, DeltaTime, false, 1);
		}
	}
	
	GEngine->AddOnScreenDebugMessage(uint64(this), 1.0f, FColor::Green, OnScreenDebugString);
}
