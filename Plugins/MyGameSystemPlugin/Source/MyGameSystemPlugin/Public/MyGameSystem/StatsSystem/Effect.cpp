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
		if (IsValid(GetRelatedStatsComponent()))
		{
			GetRelatedStatsComponent()->AddEffect(this);

			OnActivated();
			Notify_OnActivated();
		}
	}
}

void UEffect::Deactivate()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(GetRelatedStatsComponent()))
		{
			OnDeactivating();

			GetRelatedStatsComponent()->RemoveEffect(this);
		}

		this->Destroy();
	}
}

UStatsComponent* UEffect::GetRelatedStatsComponent() const
{
	if (IsValid(GetOuter()))
	{
		return Cast<UStatsComponent>(GetOuter());
	}
	
	return nullptr;
}

AActor* UEffect::GetRelatedActor() const
{
	if (IsValid(GetRelatedStatsComponent()))
	{
		return GetRelatedStatsComponent()->GetOwner();
	}
	
	return nullptr;
}

void UEffect::EndPlay_Implementation()
{
	Broadcast_OnDeactivated();

	Super::EndPlay_Implementation();
}

void UEffect::Notify_OnActivated_Implementation()
{
	Broadcast_OnActivated();
}

void UEffect::Broadcast_OnActivated()
{
	OnEffectActivated.Broadcast(this);
}

void UEffect::Broadcast_OnDeactivated()
{
	OnEffectDeactivated.Broadcast(this);
}
