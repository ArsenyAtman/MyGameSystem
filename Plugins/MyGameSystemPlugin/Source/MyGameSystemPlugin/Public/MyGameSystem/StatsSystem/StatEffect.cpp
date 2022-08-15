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
			AddRelatedStatByName(RelatedStats, ForStatOfName);
			break;
		
		case ERelatedStatsSearchType::SearchByClassOnly:
			AddRelatedStatsByClass(RelatedStats, ForStatsOfClass);
			break;

		case ERelatedStatsSearchType::SearchByNameAndByClass:
			AddRelatedStatByName(RelatedStats, ForStatOfName);
			AddRelatedStatsByClass(RelatedStats, ForStatsOfClass);
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

	Super::OnDeactivating_Implementation();
}

void UStatEffect::AddRelatedStatByName(TArray<UStat*>& RelatedStats, FName Name) const
{
	UStat* FoundStat = GetRelatedStatsComponent()->GetStatByName(Name);
	if(IsValid(FoundStat))
	{
		RelatedStats.Add(FoundStat);
	}
}

void UStatEffect::AddRelatedStatsByClass(TArray<UStat*>& RelatedStats, TSubclassOf<UStat> Class) const
{
	RelatedStats.Append(GetRelatedStatsComponent()->GetStatsOfClass(Class));
}
