// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "Stage.h"
#include "QuestComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UQuest::Activate_Implementation(UQuestComponent* QuestComponent)
{
	OwningQuestComponent = QuestComponent;
	if (IsValid(InitialStageClass))
	{
		ActiveStage = NewObject<UStage>(this, InitialStageClass);
		if (IsValid(ActiveStage))
		{
			QuestInfo.Condition = ETaskCondition::InProcess;
			ActiveStage->Activate(this);
		}
	}

	OnQuestActivated();
}

void UQuest::SetIsBeingTracked(bool bNewIsBeingTracked)
{
	if (QuestInfo.Condition == ETaskCondition::InProcess)
	{
		if (bNewIsBeingTracked)
		{
			ActiveStage->MarkObjectives();
		}
		else
		{
			ActiveStage->UnmarkObjectives();
		}

		QuestInfo.bIsBeingTracked = bNewIsBeingTracked;
	}
}

void UQuest::StagePassed_Implementation(UStage* Stage, TSubclassOf<UStage> NextStageClass)
{
	if (Stage == ActiveStage)
	{
		PastStages.Add(ActiveStage);
		if (IsValid(NextStageClass))
		{
			ActiveStage = NewObject<UStage>(this, NextStageClass);
			ActiveStage->Activate(this);
			SetIsBeingTracked(QuestInfo.bIsBeingTracked);

			Update();
		}
		else
		{
			if (ActiveStage->GetStageInfo().Condition == ETaskCondition::Completed)
			{
				Complete();
			}
			else
			{
				Fail();
			}
		}
	}
}

void UQuest::Update_Implementation()
{
	OnQuestUpdated();
	OwningQuestComponent->QuestUpdated(this);
}

FQuestInfo UQuest::GetQuestInfo() const
{
	FQuestInfo QuestInfoToReturn = QuestInfo;
	QuestInfoToReturn.CurrentStageInfo = ActiveStage->GetStageInfo();
	QuestInfoToReturn.PastStagesInfo.Empty();
	for (UStage* Stage : PastStages)
	{
		QuestInfoToReturn.PastStagesInfo.Add(Stage->GetStageInfo());
	}
	return QuestInfoToReturn;
}

void UQuest::Complete_Implementation()
{
	if (IsValid(NextQuestIfCompleted))
	{
		OwningQuestComponent->AddQuest(NextQuestIfCompleted);
	}
	SetIsBeingTracked(false);
	QuestInfo.Condition = ETaskCondition::Completed;
	OnQuestCompleted();
	OwningQuestComponent->QuestCompleted(this);
}

void UQuest::Fail_Implementation()
{
	if (IsValid(NextQuestIfFailed))
	{
		OwningQuestComponent->AddQuest(NextQuestIfFailed);
	}
	SetIsBeingTracked(false);
	QuestInfo.Condition = ETaskCondition::Failed;
	OnQuestFailed();
	OwningQuestComponent->QuestFailed(this);
}
