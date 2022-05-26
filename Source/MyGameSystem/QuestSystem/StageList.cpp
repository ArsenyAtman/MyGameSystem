// Fill out your copyright notice in the Description page of Project Settings.


#include "StageList.h"

#include "Objective.h"
#include "Net/UnrealNetwork.h"

void UStageList::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStageList, ActiveObjectives);
}

void UStageList::ActivateObjectives_Implementation()
{
	for (UClass* ObjectiveClass : InitialObjectives)
	{
		if (IsValid(ObjectiveClass) && ObjectiveClass->IsChildOf(UObjective::StaticClass()))
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
}

void UStageList::AbortAllObjectives_Implementation()
{
	for (UObjective* Objective : ActiveObjectives)
	{
		Objective->Abort();
	}

	Super::AbortAllObjectives_Implementation();
}

TArray<class UObjective*> UStageList::GetStageObjectives_Implementation() const
{
	return ActiveObjectives;
}