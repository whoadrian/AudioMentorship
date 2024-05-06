// Fill out your copyright notice in the Description page of Project Settings.


#include "Mentorship_01_ScifiBase/Public/AudioAmbiencePlayerComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Mentorship_01_ScifiBase/Public/AudioAmbienceSubsystem.h"


UAudioAmbiencePlayerComponent::UAudioAmbiencePlayerComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
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

void UAudioAmbiencePlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	if (!bAmbienceRayEnabled)
	{
		for (AAudioAmbienceZone* Zone : AmbienceRayZones)
		{
			GetWorld()->DestroyActor(Zone);
		}

		return;
	}

	if (AmbienceRayCount > AmbienceRayZones.Num())
	{
		const FTransform SpawnTransform(FRotator::ZeroRotator, GetOwner()->GetActorLocation());

		for (int Index=0; Index<AmbienceRayCount - AmbienceRayZones.Num(); ++Index)
		{
			AAudioAmbienceZone* NewZone = Cast<AAudioAmbienceZone>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AAudioAmbienceZone::StaticClass(), SpawnTransform));
			if (NewZone != nullptr)
			{
				NewZone->ZoneTag = AmbienceRayZoneTag;
				UGameplayStatics::FinishSpawningActor(NewZone, SpawnTransform);
				
				AmbienceRayZones.Add(NewZone);
			}
		}
	}
	else
	{
		for (int Index=0; Index<AmbienceRayZones.Num() - AmbienceRayCount; ++Index)
		{
			GetWorld()->DestroyActor(AmbienceRayZones.Pop());
		}
	}

	if (AmbienceRayZones.Num() == 0)
	{
		return;
	}

	for (int Index=0; Index<AmbienceRayZones.Num(); ++ Index)
	{
		const float Angle = Index * (360.0f / AmbienceRayZones.Num());
		const FVector Direction = FVector::ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);
		
		const FVector StartLocation = GetOwner()->GetActorLocation() + AmbienceRaySpawnOffset + Direction * AmbienceRayMinDistance;
		const FVector EndLocation = StartLocation + Direction * AmbienceRayMaxDistance;

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(GetOwner());

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams);

		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1, 2);

		if (bHit && HitResult.bBlockingHit)
		{
			AmbienceRayZones[Index]->SetActorLocation(HitResult.ImpactPoint);
		}
		else
		{
			AmbienceRayZones[Index]->SetActorLocation(EndLocation);
		}
	}
}
