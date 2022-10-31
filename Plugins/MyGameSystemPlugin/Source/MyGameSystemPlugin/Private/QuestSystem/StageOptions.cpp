// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/StageOptions.h"

#include "QuestSystem/Objective.h"
#include "Net/UnrealNetwork.h"

void UStageOptions::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStageOptions, Options);
	DOREPLIFETIME(UStageOptions, StageObjectives);
}

void UStageOptions::ActivateObjectives_Implementation()
{
	for (FOption& Option : Options)
	{
		UObjective* NewObjective = NewObject<UObjective>(this, Option.ObjectiveClass);
		if (IsValid(NewObjective))
		{
			StageObjectives.Add(NewObjective);
			NewObjective->Activate();
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
}

void UStageOptions::AbortAllObjectives_Implementation()
{
	for (UObjective* Objective : GetStageObjectives())
	{
		Objective->Abort();
	}

	Super::AbortAllObjectives_Implementation();
}

UClass* UStageOptions::GetNextStage(UObjective* OfObjective, const TArray<FOption>& FromOptions) const
{
	TArray<UObjective*> Objectives = GetStageObjectives();
	for (int16 i = 0; i < Options.Num(); ++i)
	{
		if(Objectives[i] == OfObjective)
		{
			return Options[i].NextStageClass;
		}
	}
	return nullptr;
}

UObjective* UStageOptions::GetCompletedObjective(const TArray<UObjective*>& Objectives) const
{
	for (UObjective* Objective : GetStageObjectives())
	{
		if (Objective->GetCondition() == ETaskCondition::Completed && !Objective->GetIsOptional())
		{
			return Objective;
		}
	}
	return nullptr;
}