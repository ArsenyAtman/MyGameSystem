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

	DOREPLIFETIME(UStatsComponent, Stats);
	DOREPLIFETIME(UStatsComponent, Effects);
}

void UStatsComponent::ApplyEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetOuter() == this)
		{
			Effect->Activate();
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

			Broadcast_OnEffectAdded(Effect);
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

			Broadcast_OnEffectRemoved(Effect);
		}
	}
}

bool UStatsComponent::AddStat(UStat* NewStat)
{
	Stats.Add(NewStat);
	return true;
}

bool UStatsComponent::RemoveStat(UStat* Stat)
{
	if(Stats.Find(Stat) != INDEX_NONE)
	{
		Stats.Remove(Stat);
		return true;
	}

	return false;
}

TArray<UEffect*> UStatsComponent::GetEffectsOfClass(TSubclassOf<UEffect> EffectClass) const
{
	TArray<UEffect*> FoundEffects;

	if (IsValid(EffectClass))
	{
		for (UEffect* Effect : GetEffects())
		{
			if (Effect->GetClass()->IsChildOf(EffectClass))
			{
				FoundEffects.Add(Effect);
			}
		}
	}

	return FoundEffects;
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

UStat* UStatsComponent::GetStatByName(FName StatName) const
{
	for (UStat* Stat : GetStats())
	{
		if(Stat->GetStatName() == StatName)
		{
			return Stat;
		}
	}

	return nullptr;
}

void UStatsComponent::OnRep_Effects(const TArray<UEffect*>& PreReplicationEffects)
{
	BroadcastChange_Effects(PreReplicationEffects);
}

void UStatsComponent::BroadcastChange_Effects(const TArray<UEffect*>& PrevEffects)
{
	TArray<UEffect*> AddedEffects = FindMissingEffects(Effects, PrevEffects);
	TArray<UEffect*> RemovedEffects = FindMissingEffects(PrevEffects, Effects);

	for(UEffect* Effect : AddedEffects)
	{
		Broadcast_OnEffectAdded(Effect);
	}

	for(UEffect* Effect : RemovedEffects)
	{
		Broadcast_OnEffectRemoved(Effect);
	}
}

void UStatsComponent::Broadcast_OnEffectAdded(UEffect* Effect)
{
	OnEffectAdded.Broadcast(Effect);
}

void UStatsComponent::Broadcast_OnEffectRemoved(UEffect* Effect)
{
	OnEffectRemoved.Broadcast(Effect);
}

TArray<UEffect*> UStatsComponent::FindMissingEffects(const TArray<UEffect*>& FromArray, const TArray<UEffect*>& InArray) const
{
	TArray<UEffect*> MissingEffects;

	for(UEffect* Effect : FromArray)
	{
		if(InArray.Find(Effect) == INDEX_NONE)
		{
			MissingEffects.Add(Effect);
		}
	}

	return MissingEffects;
}
