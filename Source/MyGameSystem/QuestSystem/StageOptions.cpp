// Fill out your copyright notice in the Description page of Project Settings.


#include "StageOptions.h"
#include "Objective.h"

void UStageOptions::Activate_Implementation(UQuest* RelatedQuest)
{
	Super::Activate_Implementation(RelatedQuest);

	for (FOption& Option : Options)
	{
		UObjective* NewObjective = NewObject<UObjective>(this, Option.ObjectiveClass);
		if (IsValid(NewObjective))
		{
			Option.Objective = NewObjective;
			Option.Objective->Activate(this);
		}
	}
}

void UStageOptions::CheckCondition_Implementation()
{
	TArray<UObjective*> Objectives = GetStageObjectives();
	if (IsOneObjectiveCompleted(Objectives))
	{
		UObjective* Objective = GetCompletedObjective(Objectives);
		Complete(GetNextStage(Objective, Options));
	}
	else if (IsAllObjectivesFailed(Objectives))
	{
		Fail(NextStageClassIfFailed);
	}
	else
	{
		Update();
	}
}

void UStageOptions::AbortAllObjectives_Implementation()
{
	for (FOption& Option : Options)
	{
		Option.Objective->Abort();
	}
}

TArray<class UObjective*> UStageOptions::GetStageObjectives_Implementation()
{
	TArray<UObjective*> Objectives;
	for (FOption& Option : Options)
	{
		Objectives.Add(Option.Objective);
	}
	return Objectives;
}

class UClass* UStageOptions::GetNextStage(UObjective* OfObjective, TArray<FOption> FromOptions)
{
	for (FOption& Option : Options)
	{
		if (Option.Objective == OfObjective)
		{
			return Option.NextStageClass;
		}
	}
	return nullptr;
}

UObjective* UStageOptions::GetCompletedObjective(TArray<UObjective*> Objectives)
{
	for (UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition == ETaskCondition::Completed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return Objective;
		}
	}
	return nullptr;
}