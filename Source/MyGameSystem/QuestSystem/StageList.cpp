// Fill out your copyright notice in the Description page of Project Settings.


#include "StageList.h"
#include "Objective.h"

void UStageList::Activate_Implementation(UQuest* RelatedQuest)
{
	Super::Activate_Implementation(RelatedQuest);

	for (UClass* ObjectiveClass : InitialObjectives)
	{
		if (IsValid(ObjectiveClass))
		{
			if (ObjectiveClass->IsChildOf(UObjective::StaticClass()))
			{
				UObjective* NewObjective = NewObject<UObjective>(this, ObjectiveClass);
				if (IsValid(NewObjective))
				{
					ActiveObjectives.Add(NewObjective);
					NewObjective->Activate(this);
				}
			}
		}
	}
}

void UStageList::CheckCondition_Implementation()
{
	if (IsAllObjectivesCompleted(ActiveObjectives))
	{
		Complete(NextStageIfCompleted);
	}
	else if (IsOneObjectiveFailed(ActiveObjectives))
	{
		Fail(NextStageIfFailed);
	}
	else
	{
		Update();
	}
}

void UStageList::AbortAllObjectives_Implementation()
{
	for (UObjective* Objective : ActiveObjectives)
	{
		Objective->Abort();
	}
}

TArray<class UObjective*> UStageList::GetStageObjectives_Implementation()
{
	return ActiveObjectives;
}