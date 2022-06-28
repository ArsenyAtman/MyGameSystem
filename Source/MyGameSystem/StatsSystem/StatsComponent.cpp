// Fill out your copyright notice in the Description page of Project Settings.

#include "StatsComponent.h"
#include "Effect.h"
#include "Stat.h"
#include "Net/UnrealNetwork.h"
#include "MyGameSystem/ArrayFunctionLibrary/ArrayFunctionLibrary.h"


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

	DOREPLIFETIME(UStatsComponent, Effects);
	DOREPLIFETIME(UStatsComponent, Stats);
}

void UStatsComponent::ApplyEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect))
		{
			Effect->ChangeOuter(this);
			Effect->Activate();
		}
	}
}

void UStatsComponent::AbortEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetRelatedStatsComponent() == this)
		{
			Effect->Deactivate();
		}
	}
}

void UStatsComponent::AddEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetRelatedStatsComponent() == this)
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
		if (IsValid(Effect) && Effect->GetRelatedStatsComponent() == this)
		{
			Effects.Remove(Effect);

			Broadcast_OnEffectRemoved(Effect);
		}
	}
}

bool UStatsComponent::AddStat(UStat* NewStat)
{
	if(IsValid(NewStat))
	{
		NewStat->ChangeOuter(this);
		Stats.Add(NewStat);
		Broadcast_OnStatAdded(NewStat);
		return true;
	}

	return false;
}

bool UStatsComponent::RemoveStat(UStat* Stat)
{
	if(Stats.Find(Stat) != INDEX_NONE)
	{
		Stats.Remove(Stat);
		Broadcast_OnStatRemoved(Stat);
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
	TArray<UEffect*> AddedEffects = UArrayFunctionLibrary::FindMissing(Effects, PrevEffects);
	TArray<UEffect*> RemovedEffects = UArrayFunctionLibrary::FindMissing(PrevEffects, Effects);

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

void UStatsComponent::OnRep_Stats(const TArray<UStat*>& PreReplicationStats)
{
	BroadcastChange_Stats(PreReplicationStats);
}

void UStatsComponent::BroadcastChange_Stats(const TArray<UStat*>& PrevStats)
{
	TArray<UStat*> AddedStats = UArrayFunctionLibrary::FindMissing(Stats, PrevStats);
	TArray<UStat*> RemovedStats = UArrayFunctionLibrary::FindMissing(PrevStats, Stats);

	for(UStat* Stat : AddedStats)
	{
		Broadcast_OnStatAdded(Stat);
	}

	for(UStat* Stat : RemovedStats)
	{
		Broadcast_OnStatRemoved(Stat);
	}
}

void UStatsComponent::Broadcast_OnStatAdded(UStat* Stat)
{
	OnStatAdded.Broadcast(Stat);
}

void UStatsComponent::Broadcast_OnStatRemoved(UStat* Stat)
{
	OnStatRemoved.Broadcast(Stat);
}
