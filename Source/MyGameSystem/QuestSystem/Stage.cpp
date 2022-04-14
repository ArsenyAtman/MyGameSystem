// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage.h"
#include "Quest.h"
#include "Objective.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UStage::Activate_Implementation(UQuest* RelatedQuest)
{
	OwningQuest = RelatedQuest;
	Condition = ETaskCondition::InProcess;
	ActivateObjectives();
	OnStageActivated();
}

void UStage::ObjectiveCompleted_Implementation(class UObjective* Objective)
{
	if (Condition == ETaskCondition::InProcess)
	{
		CheckCondition();
	}
}

void UStage::ObjectiveFailed_Implementation(class UObjective* Objective)
{
	if (Condition == ETaskCondition::InProcess)
	{
		CheckCondition();
	}
}

void UStage::Update_Implementation()
{
	OnStageUpdated();
	OwningQuest->Update();
}

void UStage::MarkObjectives()
{
	for (UObjective* Objective : GetStageObjectives())
	{
		if (Objective->GetObjectiveInfo().Condition == ETaskCondition::InProcess)
		{
			Objective->Mark();
		}
	}
}

void UStage::UnmarkObjectives()
{
	for (UObjective* Objective : GetStageObjectives())
	{
		Objective->Unmark();
	}
}

FStageInfo UStage::GetStageInfo() const
{
	TArray<UObjective*> Objectives = GetStageObjectives();
	TArray<FObjectiveInfo> ObjectiveInfos;
	for (const UObjective* Objective : Objectives)
	{
		ObjectiveInfos.Add(Objective->GetObjectiveInfo());
	}
	return FStageInfo(StageData, Condition, ObjectiveInfos);
}

void UStage::Complete_Implementation(TSubclassOf<UStage> NextStage)
{
	Condition = ETaskCondition::Completed;
	AbortAllObjectives();
	OnStageCompleted();
	OwningQuest->StagePassed(this, NextStage);
}

void UStage::Fail_Implementation(TSubclassOf<UStage> NextStage)
{
	Condition = ETaskCondition::Failed;
	AbortAllObjectives();
	OnStageFailed();
	OwningQuest->StagePassed(this, NextStage);
}

bool UStage::IsAllObjectivesCompleted(const TArray<UObjective*>& Objectives) const
{
	for (const UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition != ETaskCondition::Completed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return false;
		}
	}
	return true;
}

bool UStage::IsAllObjectivesFailed(const TArray<UObjective*>& Objectives) const
{
	for (const UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition != ETaskCondition::Failed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return false;
		}
	}
	return true;
}

bool UStage::IsOneObjectiveCompleted(const TArray<UObjective*>& Objectives) const
{
	for (const UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition == ETaskCondition::Completed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return true;
		}
	}
	return false;
}

bool UStage::IsOneObjectiveFailed(const TArray<UObjective*>& Objectives) const
{
	for (const UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition == ETaskCondition::Failed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return true;
		}
	}
	return false;
}
