// Fill out your copyright notice in the Description page of Project Settings.


#include "Stat.h"
#include "Effect.h"
#include "StatDeltaApplier.h"
#include "StatsComponent.h"
#include "Effect.h"
#include "Net/UnrealNetwork.h"
#include "MyGameSystem/ArrayFunctionLibrary/ArrayFunctionLibrary.h"

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

		Changed_StatValues(PrevValues);
	}
}

void UStat::OnRep_StatValues(FStatValues PreReplicationStatValues)
{
	Changed_StatValues(PreReplicationStatValues);
}

void UStat::Changed_StatValues(FStatValues PrevValues)
{
	FStatValues CurrentValues = GetStatValues();

	if(PrevValues != CurrentValues)
	{
		if(HasAuthority())
		{
			ValuesChanged(PrevValues);
		}

		Broadcast_OnValuesChanged(PrevValues);

		if(CurrentValues.Value == CurrentValues.Min && (PrevValues.Value != CurrentValues.Value || PrevValues.Min != CurrentValues.Min))
		{
			if(HasAuthority())
			{
				MinReached(PrevValues);
			}

			Broadcast_OnMinReached(PrevValues);
		}

		if(CurrentValues.Value == CurrentValues.Max && (PrevValues.Value != CurrentValues.Value || PrevValues.Max != CurrentValues.Max))
		{
			if(HasAuthority())
			{
				MaxReached(PrevValues);
			}
			
			Broadcast_OnMaxReached(PrevValues);
		}
	}
}

void UStat::Broadcast_OnValuesChanged(FStatValues PrevValues)
{
	OnValuesChanged.Broadcast(this, PrevValues);
}

void UStat::Broadcast_OnMinReached(FStatValues PrevValues)
{
	OnMinReached.Broadcast(this, PrevValues);
}

void UStat::Broadcast_OnMaxReached(FStatValues PrevValues)
{
	OnMaxReached.Broadcast(this, PrevValues);
}

void UStat::OnRep_Effects(const TArray<UEffect*>& PreReplicationEffects)
{
	Broadcast_Effects(PreReplicationEffects);
}

void UStat::Broadcast_Effects(const TArray<UEffect*>& PrevEffects)
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

void UStat::Broadcast_OnEffectAdded(UEffect* Effect)
{
	OnEffectAdded.Broadcast(this, Effect);
}

void UStat::Broadcast_OnEffectRemoved(UEffect* Effect)
{
	OnEffectRemoved.Broadcast(this, Effect);
}
