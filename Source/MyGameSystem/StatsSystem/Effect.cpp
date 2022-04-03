// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect.h"
#include "StatsComponent.h"
#include "Stat.h"
#include "EffectDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"

void UEffect::Activate(UStatsComponent* StatsComponent)
{
	if (!GetIsActive() && IsValid(StatsComponent) && GetRelatedStatsComponent() == nullptr)
	{
		bIsActive = true;

		RelatedStatsComponent = StatsComponent;
		RelatedStatsComponent->AddEffect(this);

		OnActivated();
	}
}

void UEffect::Deactivate()
{
	if (GetIsActive())
	{
		OnDeactivating();

		bIsActive = false;

		RelatedStatsComponent->RemoveEffect(this);
		RelatedStatsComponent = nullptr;
	}
}

AActor* UEffect::GetRelatedActor()
{
	if (IsValid(GetRelatedStatsComponent()))
	{
		return GetRelatedStatsComponent()->GetOwner();
	}
	else
	{
		return nullptr;
	}
}

FEffectInfo UEffect::GetEffectInfo()
{
	return FEffectInfo(EffectData);
}