// Fill out your copyright notice in the Description page of Project Settings.


#include "StatEffect.h"
#include "StatsComponent.h"
#include "Stat.h"

void UStatEffect::OnActivated_Implementation()
{
	Super::OnActivated_Implementation();

	RelatedStats = GetRelatedStatsComponent()->GetStatsOfClass(ForStatsOfClass);
	for (UStat* Stat : GetRelatedStats())
	{
		Stat->AddEffect(this);
	}
}

void UStatEffect::OnDeactivating_Implementation()
{
	for (UStat* Stat : GetRelatedStats())
	{
		Stat->RemoveEffect(this);
	}
	RelatedStats.Empty();

	Super::OnDeactivating_Implementation();
}