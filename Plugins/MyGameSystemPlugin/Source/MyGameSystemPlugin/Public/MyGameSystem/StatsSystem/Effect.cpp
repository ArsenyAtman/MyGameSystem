// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect.h"
#include "StatsComponent.h"
#include "Stat.h"
#include "Net/UnrealNetwork.h"

void UEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ...
}

void UEffect::Activate()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetRelatedEffectsComponent()))
		{
			GetRelatedEffectsComponent()->AddEffect(this);

			Notify_OnActivated();
		}
	}
}

void UEffect::Deactivate()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetRelatedEffectsComponent()))
		{
			Notify_OnDeactivating();

			GetRelatedEffectsComponent()->RemoveEffect(this);
		}

		this->Destroy();
	}
}

UEffectsComponent* UEffect::GetRelatedEffectsComponent() const
{
	if (IsValid(GetOuter()))
	{
		return Cast<UEffectsComponent>(GetOuter());
	}
	
	return nullptr;
}

AActor* UEffect::GetRelatedActor() const
{
	if (IsValid(GetRelatedEffectsComponent()))
	{
		return GetRelatedEffectsComponent()->GetOwner();
	}
	
	return nullptr;
}

void UEffect::Notify_OnActivated_Implementation()
{
	OnActivated();
	Broadcast_OnActivated();
}

void UEffect::Notify_OnDeactivating_Implementation()
{
	OnDeactivating();
	Broadcast_OnDeactivating();
}

void UEffect::Broadcast_OnActivated()
{
	OnEffectActivated.Broadcast(this);
}

void UEffect::Broadcast_OnDeactivating()
{
	OnEffectDeactivating.Broadcast(this);
}
