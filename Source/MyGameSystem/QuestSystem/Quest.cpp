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
			Condition = ETaskCondition::InProcess;
			ActiveStage->Activate(this);
		}
	}

	OnQuestActivated();
}

void UQuest::SetIsBeingTracked(bool bNewIsBeingTracked)
{
	if (Condition == ETaskCondition::InProcess)
	{
		if (bNewIsBeingTracked)
		{
			ActiveStage->MarkObjectives();
		}
		else
		{
			ActiveStage->UnmarkObjectives();
		}

		bIsBeingTracked = bNewIsBeingTracked;
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
			SetIsBeingTracked(bIsBeingTracked);

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
	TArray<FStageInfo> PastStageInfos;
	for (const UStage* Stage : PastStages)
	{
		PastStageInfos.Add(Stage->GetStageInfo());
	}
	return FQuestInfo(QuestData, Condition, bIsBeingTracked, ActiveStage->GetStageInfo(), PastStageInfos);
}

void UQuest::Complete_Implementation()
{
	if (IsValid(NextQuestIfCompleted))
	{
		OwningQuestComponent->AddQuest(NextQuestIfCompleted);
	}
	SetIsBeingTracked(false);
	Condition = ETaskCondition::Completed;
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
	Condition = ETaskCondition::Failed;
	OnQuestFailed();
	OwningQuestComponent->QuestFailed(this);
}
