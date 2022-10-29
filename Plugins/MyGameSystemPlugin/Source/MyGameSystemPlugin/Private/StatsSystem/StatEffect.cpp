// Fill out your copyright notice in the Description page of Project Settings.


#include "StatsSystem/StatEffect.h"

#include "StatsSystem/StatsComponent.h"
#include "StatsSystem/Stat.h"

UStatsComponent* UStatEffect::GetRelatedStatsComponent() const
{
	return Cast<UStatsComponent>(GetRelatedEffectsComponent());
}

void UStatEffect::OnActivated_Implementation()
{
	Super::OnActivated_Implementation();

	if (IsValid(GetRelatedStatsComponent()))
	{
		RelatedStats = GetRelatedStatsComponent()->GetStatsOfClass(ForStatsOfClass);

		GetRelatedStatsComponent()->OnStatAdded.AddDynamic(this, &UStatEffect::OnStatAdded);
		GetRelatedStatsComponent()->OnStatRemoved.AddDynamic(this, &UStatEffect::OnStatRemoved);
	}

	for (UStat* Stat : GetRelatedStats())
	{
		if(IsValid(Stat))
		{
			Stat->AddEffect(this);
		}
	}
}

void UStatEffect::OnDeactivating_Implementation()
{
	for (UStat* Stat : GetRelatedStats())
	{
		if(IsValid(Stat))
		{
			Stat->RemoveEffect(this);
		}
	}

	if (IsValid(GetRelatedStatsComponent()))
	{
		GetRelatedStatsComponent()->OnStatAdded.RemoveDynamic(this, &UStatEffect::OnStatAdded);
		GetRelatedStatsComponent()->OnStatRemoved.RemoveDynamic(this, &UStatEffect::OnStatRemoved);
	}

	Super::OnDeactivating_Implementation();
}

void UStatEffect::OnStatAdded(class UStat* Stat)
{
	RelatedStats.Add(Stat);
}

void UStatEffect::OnStatRemoved(class UStat* Stat)
{
	RelatedStats.Remove(Stat);
}
