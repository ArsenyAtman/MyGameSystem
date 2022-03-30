// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat.h"
#include "Effect.h"
#include "StatDeltaApplier.h"
#include "StatsComponent.h"
#include "Effect.h"
#include "Kismet/KismetSystemLibrary.h"

FStatValues UStat::ApplyDelta(FStatValues Delta, UEffect* Effect)
{
	if (IsValid(StatDeltaApplier))
	{
		FStatValues ActualDelta = StatDeltaApplier->ApplyDelta(Delta, Effect);
		StatValues = StatValues + ActualDelta;

		FStatValues OldStatValues = StatValues;

		StatValues.Value = FMath::Clamp(StatValues.Value, StatValues.Min, StatValues.Max);

		ActualDelta = StatValues - OldStatValues;

		if (OnValuesChanged.IsBound())
		{
			OnValuesChanged.Broadcast(ActualDelta, Effect);
		}

		if (StatValues.Min >= StatValues.Value)
		{
			if (OnMinReached.IsBound())
			{
				OnMinReached.Broadcast();
			}
		}
		else if (StatValues.Max <= StatValues.Value)
		{
			if (OnMaxReached.IsBound())
			{
				OnMaxReached.Broadcast();
			}
		}

		return ActualDelta;
	}
	else
	{
		return FStatValues();
	}
}

void UStat::AddEffect(UEffect* Effect)
{
	if (IsValid(Effect))
	{
		Effects.Add(Effect);
	}

	if (OnEffectAdded.IsBound())
	{
		OnEffectAdded.Broadcast(Effect);
	}
}

void UStat::RemoveEffect(UEffect* Effect)
{
	if (IsValid(Effect))
	{
		Effects.Remove(Effect);
	}

	if (OnEffectRemoved.IsBound())
	{
		OnEffectRemoved.Broadcast(Effect);
	}
}

UStatsComponent* UStat::GetOwningStatsComponent()
{
	return Cast<UStatsComponent>(GetOuter());
}

FStatInfo UStat::GetStatInfo()
{
	TArray<FEffectInfo> EffectInfos;

	for (UEffect* Effect : GetEffects())
	{
		EffectInfos.Add(Effect->GetEffectInfo());
	}

	return FStatInfo(GetClass(), StatData, StatBaseValues, StatValues, EffectInfos);
}