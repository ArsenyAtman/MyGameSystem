// Fill out your copyright notice in the Description page of Project Settings.

#include "Objective.h"

#include "Stage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectivesManager.h"
#include "QuestComponent.h"

void UObjective::Activate_Implementation(UStage* RelatedStage)
{
	OwningStage = RelatedStage;
	ObjectiveInfo.Condition = ETaskCondition::InProcess;

	OnLevelObjectives = GetOnLevelObjectivesFormManager();
}

void UObjective::Abort_Implementation()
{
	if (ObjectiveInfo.Condition == ETaskCondition::InProcess)
	{
		ObjectiveInfo.Condition = ETaskCondition::Aborted;
		Unmark();
	}
}

void UObjective::Mark()
{
	MarkObjectives(OnLevelObjectives);
}

void UObjective::Unmark()
{
	UnmarkObjectives(OnLevelObjectives);
}

void UObjective::Update_Implementation()
{
	OwningStage->Update();
}

FObjectiveInfo UObjective::GetObjectiveInfo()
{
	return ObjectiveInfo;
}

void UObjective::Fail_Implementation()
{
	ObjectiveInfo.Condition = ETaskCondition::Failed;
	Unmark();
	OwningStage->ObjectiveFailed(this);
}

void UObjective::Complete_Implementation()
{
	ObjectiveInfo.Condition = ETaskCondition::Completed;
	Unmark();
	OwningStage->ObjectiveCompleted(this);
}

void UObjective::MarkObjectives(TArray<AActor*> Objectives)
{
	UQuestComponent* OwningQuestComponent = OwningStage->GetOwningQuest()->GetOwningQuestComponent();
	if (IsValid(OwningQuestComponent))
	{
		OwningQuestComponent->MarkActors(MarkerClass, Objectives);
	}
}

void UObjective::UnmarkObjectives(TArray<AActor*> Objectives)
{
	UQuestComponent* OwningQuestComponent = OwningStage->GetOwningQuest()->GetOwningQuestComponent();
	if (IsValid(OwningQuestComponent))
	{
		OwningQuestComponent->UnmarkActors(MarkerClass, Objectives);
	}
}

TArray<AActor*> UObjective::GetOnLevelObjectivesFormManager()
{
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectivesManager::StaticClass(), FoundManagers);
	if (FoundManagers.IsValidIndex(0))
	{
		AObjectivesManager* ObjectivesManager = Cast<AObjectivesManager>(FoundManagers[0]);
		if (IsValid(ObjectivesManager))
		{
			return ObjectivesManager->GetOnLevelObjectives(this);
		}
	}
	return TArray<AActor*>();
}