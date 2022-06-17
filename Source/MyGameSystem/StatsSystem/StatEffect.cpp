// Fill out your copyright notice in the Description page of Project Settings.


#include "StatEffect.h"
#include "StatsComponent.h"
#include "Stat.h"

TArray<UStat*> UStatEffect::GetRelatedStats() const
{
	TArray<UStat*> RelatedStats;

	switch(RelatedStatsSearchType)
	{
		case ERelatedStatsSearchType::SearchByNameOnly:
			RelatedStats.Add(GetRelatedStatsComponent()->GetStatByName(ForStatOfName));
			break;
		
		case ERelatedStatsSearchType::SearchByClassOnly:
			RelatedStats.Append(GetRelatedStatsComponent()->GetStatsOfClass(ForStatsOfClass));
			break;

		case ERelatedStatsSearchType::SearchByNameAndByClass:
			RelatedStats.Add(GetRelatedStatsComponent()->GetStatByName(ForStatOfName));
			RelatedStats.Append(GetRelatedStatsComponent()->GetStatsOfClass(ForStatsOfClass));
			break;

		default:
			break;
	}

	return RelatedStats;
}

void UStatEffect::OnActivated_Implementation()
{
	Super::OnActivated_Implementation();

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

	Super::OnDeactivating_Implementation();
}