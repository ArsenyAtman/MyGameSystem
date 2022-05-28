// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "Stage.h"
#include "QuestComponent.h"
#include "Net/UnrealNetwork.h"

void UQuest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UQuest, Condition);
	DOREPLIFETIME(UQuest, bIsBeingTracked);
	DOREPLIFETIME(UQuest, ActiveStage);
	DOREPLIFETIME(UQuest, PastStages);
}

void UQuest::Activate_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(InitialStageClass))
		{
			ChangeActiveStage(InitialStageClass);
			SetCondition(ETaskCondition::InProcess);
		}

		OnQuestActivated();
	}
}

void UQuest::SetIsBeingTracked(bool bNewIsBeingTracked)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			if (bNewIsBeingTracked)
			{
				GetActiveStage()->MarkObjectives();
			}
			else
			{
				GetActiveStage()->UnmarkObjectives();
			}

			bIsBeingTracked = bNewIsBeingTracked;
			BroadcastChange_IsBeingTracked();
		}
	}
}

void UQuest::StagePassed_Implementation(UStage* Stage, TSubclassOf<UStage> NextStageClass)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (Stage == GetActiveStage())
		{
			if (IsValid(NextStageClass))
			{
				ChangeActiveStage(NextStageClass);
				SetIsBeingTracked(GetIsBeingTracked());
			}
			else
			{
				if (GetActiveStage()->GetCondition() == ETaskCondition::Completed)
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
}

UQuestComponent* UQuest::GetOwningQuestComponent() const
{
	return Cast<UQuestComponent>(GetOuter());
}

void UQuest::SetCondition(ETaskCondition NewCondition)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		Condition = NewCondition;
		BroadcastChange_Condition();
	}
}

void UQuest::Complete_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			SetIsBeingTracked(false);
			SetCondition(ETaskCondition::Completed);
			OnQuestCompleted();
			GetOwningQuestComponent()->QuestCompleted(this);
			if (IsValid(NextQuestIfCompleted))
			{
				GetOwningQuestComponent()->AddQuest(NextQuestIfCompleted);
			}
		}
	}
}

void UQuest::Fail_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			SetIsBeingTracked(false);
			SetCondition(ETaskCondition::Failed);
			OnQuestFailed();
			GetOwningQuestComponent()->QuestFailed(this);
			if (IsValid(NextQuestIfFailed))
			{
				GetOwningQuestComponent()->AddQuest(NextQuestIfFailed);
			}
		}
	}
}

void UQuest::ChangeActiveStage(TSubclassOf<UStage> NewStageClass)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if(IsValid(GetActiveStage()))
		{
			PastStages.Add(GetActiveStage());
			BroadcastChange_PastStages();
		}

		if(IsValid(NewStageClass))
		{
			ActiveStage = NewObject<UStage>(this, NewStageClass);
			if(IsValid(ActiveStage))
			{
				ActiveStage->Activate();
			}
		}
		else
		{
			ActiveStage = nullptr;
		}

		BroadcastChange_ActiveStage();
	}
}

void UQuest::OnRep_ActiveStage()
{
	BroadcastChange_ActiveStage();
}

void UQuest::OnRep_PastStages()
{
	BroadcastChange_PastStages();
}

void UQuest::OnRep_Condition()
{
	BroadcastChange_Condition();
}

void UQuest::OnRep_IsBeingTracked()
{
	BroadcastChange_IsBeingTracked();
}

void UQuest::BroadcastChange_ActiveStage()
{
	OnActiveStageChanged.Broadcast(this);
}

void UQuest::BroadcastChange_PastStages()
{
	OnPastStagesChanged.Broadcast(this);
}

void UQuest::BroadcastChange_Condition()
{
	/*
	if(ConditionToDelegateMap.Contains(GetCondition()))
	{
		FQuestConditionDelegate DelegateToBroadcast = ConditionToDelegateMap[GetCondition()];
		DelegateToBroadcast.Broadcast(this);
	}
	*/

	switch(GetCondition())
	{
		case ETaskCondition::InProcess:
			OnActivated.Broadcast(this);
			break;

		case ETaskCondition::Completed:
			OnCompleted.Broadcast(this);
			break;

		case ETaskCondition::Failed:
			OnFailed.Broadcast(this);
			break;
	}
}

void UQuest::BroadcastChange_IsBeingTracked()
{
	if(GetIsBeingTracked() == true)
	{
		OnBecomeTracked.Broadcast(this);
	}
	else
	{
		OnBecomeUntracked.Broadcast(this);
	}
}
