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

void UQuest::Activate_Implementation(UQuestComponent* QuestComponent)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(InitialStageClass))
		{
			ActiveStage = NewObject<UStage>(this, InitialStageClass);
			if (IsValid(ActiveStage))
			{
				SetCondition(ETaskCondition::InProcess);
				ActiveStage->Activate(this);
			}
		}

		OnQuestActivated();
	}
}

void UQuest::SetIsBeingTracked(bool bNewIsBeingTracked)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess && bIsBeingTracked != bNewIsBeingTracked)
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
			IsBeingTrackedChangeBroadcast(bIsBeingTracked);
		}
	}
}

void UQuest::StagePassed_Implementation(UStage* Stage, TSubclassOf<UStage> NextStageClass)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (Stage == ActiveStage)
		{
			PastStages.Add(ActiveStage);
			if (IsValid(NextStageClass))
			{
				ActiveStage = NewObject<UStage>(this, NextStageClass);
				ActiveStage->Activate(this);
				SetIsBeingTracked(bIsBeingTracked);
			}
			else
			{
				if (ActiveStage->GetCondition() == ETaskCondition::Completed)
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
	Condition = NewCondition;
	ConditionChangeBroadcast(Condition);
}

void UQuest::Complete_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
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

void UQuest::Fail_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
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

void UQuest::OnRep_ActiveStage()
{
	OnActiveStageChanged.Broadcast(this);
}

void UQuest::OnRep_Condition()
{
	ConditionChangeBroadcast(GetCondition());
}

void UQuest::OnRep_IsBeingTracked()
{
	IsBeingTrackedChangeBroadcast(GetIsBeingTracked());
}

void UQuest::ConditionChangeBroadcast(ETaskCondition CurrentCondition)
{
	switch(CurrentCondition)
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

		case ETaskCondition::Aborted:

			// ...

			break;

		default:

			break;
	}
}

void UQuest::IsBeingTrackedChangeBroadcast(bool CurrentIsBeingTracked)
{
	if(CurrentIsBeingTracked == true)
	{
		OnBecomeTracked.Broadcast(this);
	}
	else
	{
		OnBecomeUntracked.Broadcast(this);
	}
}
