// Fill out your copyright notice in the Description page of Project Settings.


#include "StageOptions.h"

#include "Objective.h"
#include "Net/UnrealNetwork.h"

void UStageOptions::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStageOptions, Options);
}

void UStageOptions::ActivateObjectives_Implementation()
{
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
}

void UStageOptions::AbortAllObjectives_Implementation()
{
	for (FOption& Option : Options)
	{
		Option.Objective->Abort();
	}

	Super::AbortAllObjectives_Implementation();
}

TArray<class UObjective*> UStageOptions::GetStageObjectives_Implementation() const
{
	TArray<UObjective*> Objectives;
	for (const FOption& Option : Options)
	{
		Objectives.Add(Option.Objective);
	}
	return Objectives;
}

class UClass* UStageOptions::GetNextStage(UObjective* OfObjective, const TArray<FOption>& FromOptions) const
{
	for (const FOption& Option : FromOptions)
	{
		if (Option.Objective == OfObjective)
		{
			return Option.NextStageClass;
		}
	}
	return nullptr;
}

UObjective* UStageOptions::GetCompletedObjective(const TArray<UObjective*>& Objectives) const
{
	for (UObjective* Objective : Objectives)
	{
		if (Objective->GetCondition() == ETaskCondition::Completed && !Objective->GetIsOptional())
		{
			return Objective;
		}
	}
	return nullptr;
}