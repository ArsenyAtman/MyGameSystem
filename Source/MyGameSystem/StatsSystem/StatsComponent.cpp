// Fill out your copyright notice in the Description page of Project Settings.

#include "StatsComponent.h"
#include "Effect.h"
#include "Stat.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
}

void UStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatsComponent, EffectsInfo);
	DOREPLIFETIME(UStatsComponent, StatsInfo);
}

void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		for (UStat* Stat : GetStats())
		{
			Stat->OnValuesChanged.AddDynamic(this, &UStatsComponent::StatValuesChanged);
			Stat->OnEffectAdded.AddDynamic(this, &UStatsComponent::StatEffectAdded);
			Stat->OnEffectRemoved.AddDynamic(this, &UStatsComponent::StatEffectRemoved);
		}

		UpdateEffectsInfo();
		UpdateStatsInfo();
	}
}

void UStatsComponent::ApplyEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetOuter() == this && Effect->GetRelatedStatsComponent() == nullptr)
		{
			Effect->Activate(this);
		}
	}
}

void UStatsComponent::AbortEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Effect->GetRelatedStatsComponent() == this)
		{
			Effect->Deactivate();
		}
	}
}

void UStatsComponent::AddEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Effect->GetRelatedStatsComponent() == this)
		{
			Effects.Add(Effect);

			if (OnEffectAdded.IsBound())
			{
				OnEffectAdded.Broadcast(Effect);
			}

			UpdateEffectsInfo();
		}
	}
}

void UStatsComponent::RemoveEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Effect->GetRelatedStatsComponent() == this)
		{
			Effects.Remove(Effect);

			if (OnEffectRemoved.IsBound())
			{
				OnEffectRemoved.Broadcast(Effect);
			}

			UpdateEffectsInfo();
		}
	}
}

TArray<UStat*> UStatsComponent::GetStatsOfClass(TSubclassOf<UStat> StatClass) const
{
	TArray<UStat*> FoundStats;

	if (IsValid(StatClass))
	{
		for (UStat* Stat : GetStats())
		{
			if (Stat->GetClass()->IsChildOf(StatClass))
			{
				FoundStats.Add(Stat);
			}
		}
	}

	return FoundStats;
}

void UStatsComponent::OnRep_StatsInfo()
{
	if (OnStatsUpdated.IsBound())
	{
		OnStatsUpdated.Broadcast();
	}
}

void UStatsComponent::UpdateStatsInfo()
{
	StatsInfo.Empty();

	for (const UStat* Stat : GetStats())
	{
		StatsInfo.Add(Stat->GetStatInfo());
	}

	if (OnStatsUpdated.IsBound())
	{
		OnStatsUpdated.Broadcast();
	}
}

void UStatsComponent::OnRep_EffectsInfo()
{
	if (OnEffectsUpdated.IsBound())
	{
		OnEffectsUpdated.Broadcast();
	}
}

void UStatsComponent::UpdateEffectsInfo()
{
	EffectsInfo.Empty();

	for (const UEffect* Effect : GetEffects())
	{
		EffectsInfo.Add(Effect->GetEffectInfo());
	}

	if (OnEffectsUpdated.IsBound())
	{
		OnEffectsUpdated.Broadcast();
	}

}
