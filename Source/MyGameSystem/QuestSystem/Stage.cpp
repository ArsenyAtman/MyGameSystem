// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage.h"
#include "Quest.h"
#include "Objective.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UStage::Activate_Implementation(UQuest* RelatedQuest)
{
	OwningQuest = RelatedQuest;
	StageInfo.Condition = ETaskCondition::InProcess;
}

void UStage::ObjectiveCompleted_Implementation(class UObjective* Objective)
{
	if (StageInfo.Condition == ETaskCondition::InProcess)
	{
		CheckCondition();
	}
}

void UStage::ObjectiveFailed_Implementation(class UObjective* Objective)
{
	if (StageInfo.Condition == ETaskCondition::InProcess)
	{
		CheckCondition();
	}
}

void UStage::Update_Implementation()
{
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

FStageInfo UStage::GetStageInfo()
{
	StageInfo.ObjectivesInfo.Empty();
	TArray<UObjective*> Objectives = GetStageObjectives();
	for (UObjective* Objective : Objectives)
	{
		StageInfo.ObjectivesInfo.Add(Objective->GetObjectiveInfo());
	}
	return StageInfo;
}

void UStage::Complete_Implementation(TSubclassOf<UStage> NextStage)
{
	StageInfo.Condition = ETaskCondition::Completed;
	AbortAllObjectives();

	OwningQuest->StagePassed(this, NextStage);
}

void UStage::Fail_Implementation(TSubclassOf<UStage> NextStage)
{
	StageInfo.Condition = ETaskCondition::Failed;
	AbortAllObjectives();

	OwningQuest->StagePassed(this, NextStage);
}

TArray<AActor*> UStage::GetAllActorsOfClass(UClass* ActorClass)
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorClass, OutActors);
	return OutActors;
}

bool UStage::IsAllObjectivesCompleted(TArray<UObjective*> Objectives)
{
	for (UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition != ETaskCondition::Completed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return false;
		}
	}
	return true;
}

bool UStage::IsAllObjectivesFailed(TArray<UObjective*> Objectives)
{
	for (UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition != ETaskCondition::Failed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return false;
		}
	}
	return true;
}

bool UStage::IsOneObjectiveCompleted(TArray<UObjective*> Objectives)
{
	for (UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition == ETaskCondition::Completed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return true;
		}
	}
	return false;
}

bool UStage::IsOneObjectiveFailed(TArray<UObjective*> Objectives)
{
	for (UObjective* Objective : Objectives)
	{
		if (Objective->GetObjectiveInfo().Condition == ETaskCondition::Failed && !Objective->GetObjectiveInfo().ObjectiveData->bIsOptional)
		{
			return true;
		}
	}
	return false;
}
