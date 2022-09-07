// Fill out your copyright notice in the Description page of Project Settings.

#include "StatsComponent.h"
#include "Effect.h"
#include "Stat.h"
#include "Net/UnrealNetwork.h"

void UStatsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatsComponent, Stats);
}

bool UStatsComponent::AddStat(UStat* NewStat)
{
	if(IsValid(NewStat))
	{
		NewStat->ChangeOuter(this);
		int32 Result = Stats.AddUnique(NewStat);
		if (Result != INDEX_NONE)
		{
			Broadcast_OnStatAdded(NewStat);
			
			return true;
		}
	}

	return false;
}

bool UStatsComponent::RemoveStat(UStat* Stat)
{
	int32 Result = Stats.Remove(Stat);
	if (Result > 0)
	{
		Broadcast_OnStatRemoved(Stat);

		return true;
	}

	return false;
}

TArray<UStat*> UStatsComponent::GetStatsOfClass(TSubclassOf<UStat> StatClass) const
{
	TArray<UStat*> FoundStats;

	if (IsValid(StatClass))
	{
		for (UStat* Stat : GetStats())
		{
			if (Stat->IsA(StatClass))
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

void UStatsComponent::OnRep_Stats(const TArray<UStat*>& PreReplicationStats)
{
	BroadcastChange_Stats(PreReplicationStats);
}

void UStatsComponent::BroadcastChange_Stats(const TArray<UStat*>& PrevStats)
{
	const TArray<UStat*>& CurrentStats = Stats;
	TArray<UStat*> AddedStats = CurrentStats.FilterByPredicate([PrevStats](UStat* const& Stat){ return PrevStats.Find(Stat) == INDEX_NONE; });
	TArray<UStat*> RemovedStats = PrevStats.FilterByPredicate([CurrentStats](UStat* const& Stat){ return CurrentStats.Find(Stat) == INDEX_NONE; });

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
