// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat.h"
#include "Effect.h"
#include "StatDeltaApplier.h"
#include "StatsComponent.h"
#include "Effect.h"
#include "Net/UnrealNetwork.h"

void UStat::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStat, StatValues);
	DOREPLIFETIME(UStat, Effects);
}

FStatValues UStat::ApplyDelta(FStatValues Delta, UEffect* Effect)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(StatDeltaApplier))
		{
			FStatValues ModifiedDelta = StatDeltaApplier->ApplyDelta(Delta, Effect);
			FStatValues OldStatValues = GetStatValues();
			FStatValues NewStatValues = OldStatValues + ModifiedDelta;

			SetStatValues(NewStatValues);

			FStatValues ActualDelta = StatValues - OldStatValues;
			return ActualDelta;
		}
	}

	return FStatValues();
}

void UStat::AddEffect(UEffect* Effect)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect))
		{
			Effects.Add(Effect);

			Broadcast_OnEffectAdded(Effect);
		}
	}
}

void UStat::RemoveEffect(UEffect* Effect)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect))
		{
			Effects.Remove(Effect);

			Broadcast_OnEffectRemoved(Effect);
		}
	}
}

UStatsComponent* UStat::GetOwningStatsComponent() const
{
	return Cast<UStatsComponent>(GetOuter());
}

void UStat::SetStatValues(FStatValues NewStatValues)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		FStatValues PrevValues = StatValues;

		StatValues = NewStatValues;
		StatValues.Value = FMath::Clamp(StatValues.Value, StatValues.Min, StatValues.Max);

		BroadcastChange_StatValues(PrevValues);
	}
}

void UStat::OnRep_StatValues(FStatValues PreReplicationStatValues)
{
	BroadcastChange_StatValues(PreReplicationStatValues);
}

void UStat::BroadcastChange_StatValues(FStatValues PrevValues)
{
	FStatValues CurrentValues = GetStatValues();

	if(PrevValues != CurrentValues)
	{
		OnValuesChanged.Broadcast(this, CurrentValues - PrevValues);

		if(CurrentValues.Value == CurrentValues.Min && (PrevValues.Value != CurrentValues.Value || PrevValues.Min != CurrentValues.Min))
		{
			OnMinReached.Broadcast(this);
		}

		if(CurrentValues.Value == CurrentValues.Max && (PrevValues.Value != CurrentValues.Value || PrevValues.Max != CurrentValues.Max))
		{
			OnMaxReached.Broadcast(this);
		}
	}
}

void UStat::OnRep_Effects(const TArray<UEffect*>& PreReplicationEffects)
{
	BroadcastChange_Effects(PreReplicationEffects);
}

void UStat::BroadcastChange_Effects(const TArray<UEffect*>& PrevEffects)
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

void UStat::Broadcast_OnEffectAdded(UEffect* Effect)
{
	OnEffectAdded.Broadcast(this, Effect);
}

void UStat::Broadcast_OnEffectRemoved(UEffect* Effect)
{
	OnEffectRemoved.Broadcast(this, Effect);
}

TArray<UEffect*> UStat::FindMissingEffects(const TArray<UEffect*>& FromArray, const TArray<UEffect*>& InArray) const
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
