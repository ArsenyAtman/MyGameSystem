// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsSystem/EffectsComponent.h"

#include "StatsSystem/Effect.h"
#include "Net/UnrealNetwork.h"

UEffectsComponent::UEffectsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
}

void UEffectsComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEffectsComponent, Effects);
}

bool UEffectsComponent::AddEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effects.Find(Effect) == INDEX_NONE)
		{
			Effect->ChangeOuter(this);
			Effect->Activate();

			return true;
		}
	}

	return false;
}

bool UEffectsComponent::RemoveEffect(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetRelatedEffectsComponent() == this)
		{
			Effect->Deactivate();
			
			return true;
		}
	}

	return false;
}

void UEffectsComponent::EffectAdded(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetRelatedEffectsComponent() == this)
		{
			Effects.Add(Effect);
			Broadcast_OnEffectAdded(Effect);
		}
	}
}

void UEffectsComponent::EffectRemoved(UEffect* Effect)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Effect) && Effect->GetRelatedEffectsComponent() == this)
		{
			Effects.Remove(Effect);

			Broadcast_OnEffectRemoved(Effect);
		}
	}
}

TArray<UEffect*> UEffectsComponent::GetEffectsOfClass(TSubclassOf<UEffect> EffectClass) const
{
	TArray<UEffect*> FoundEffects;

	if (IsValid(EffectClass))
	{
		for (UEffect* Effect : GetEffects())
		{
			if (Effect->IsA(EffectClass))
			{
				FoundEffects.Add(Effect);
			}
		}
	}

	return FoundEffects;
}

void UEffectsComponent::OnRep_Effects(const TArray<UEffect*>& PreReplicationEffects)
{
	BroadcastChange_Effects(PreReplicationEffects);
}

void UEffectsComponent::BroadcastChange_Effects(const TArray<UEffect*>& PrevEffects)
{
	const TArray<UEffect*>& CurrentEffects = Effects;
	TArray<UEffect*> AddedEffects = CurrentEffects.FilterByPredicate([PrevEffects](UEffect* const& Effect){ return PrevEffects.Find(Effect) == INDEX_NONE; });
	TArray<UEffect*> RemovedEffects = PrevEffects.FilterByPredicate([CurrentEffects](UEffect* const& Effect){ return CurrentEffects.Find(Effect) == INDEX_NONE; });

	for(UEffect* Effect : AddedEffects)
	{
		Broadcast_OnEffectAdded(Effect);
	}

	for(UEffect* Effect : RemovedEffects)
	{
		Broadcast_OnEffectRemoved(Effect);
	}
}

void UEffectsComponent::Broadcast_OnEffectAdded(UEffect* Effect)
{
	OnEffectAdded.Broadcast(Effect);
}

void UEffectsComponent::Broadcast_OnEffectRemoved(UEffect* Effect)
{
	OnEffectRemoved.Broadcast(Effect);
}