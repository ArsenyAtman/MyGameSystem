// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage.h"
#include "Quest.h"
#include "Objective.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void UStage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStage, Condition);
}

void UStage::Activate_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		SetCondition(ETaskCondition::InProcess);
		ActivateObjectives();
		OnStageActivated();
	}
}

void UStage::ObjectiveCompleted_Implementation(UObjective* Objective)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			CheckCondition();
		}
	}
}

void UStage::ObjectiveFailed_Implementation(UObjective* Objective)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			CheckCondition();
		}
	}
}

void UStage::MarkObjectives()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		for (UObjective* Objective : GetStageObjectives())
		{
			if (Objective->GetCondition() == ETaskCondition::InProcess)
			{
				Objective->Mark();
			}
		}
	}
}

void UStage::UnmarkObjectives()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		for (UObjective* Objective : GetStageObjectives())
		{
			Objective->Unmark();
		}
	}
}

UQuest* UStage::GetOwningQuest() const
{
	return Cast<UQuest>(GetOuter());
}

void UStage::SetCondition(ETaskCondition NewCondition)
{
	Condition = NewCondition;
	BroadcastChange_Condition();
}

void UStage::Complete_Implementation(TSubclassOf<UStage> NextStage)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			SetCondition(ETaskCondition::Completed);
			AbortAllObjectives();
			OnStageCompleted();
			GetOwningQuest()->StagePassed(this, NextStage);
		}
	}
}

void UStage::Fail_Implementation(TSubclassOf<UStage> NextStage)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			SetCondition(ETaskCondition::Failed);
			AbortAllObjectives();
			OnStageFailed();
			GetOwningQuest()->StagePassed(this, NextStage);
		}
	}
}

void UStage::AbortAllObjectives_Implementation()
{
	// ...
}

bool UStage::IsAllObjectivesCompleted(const TArray<UObjective*>& Objectives) const
{
	for (const UObjective* Objective : Objectives)
	{
		if (Objective->GetCondition() != ETaskCondition::Completed && !Objective->GetIsOptional())
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
		if (Objective->GetCondition() != ETaskCondition::Failed && !Objective->GetIsOptional())
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
		if (Objective->GetCondition() == ETaskCondition::Completed && !Objective->GetIsOptional())
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
		if (Objective->GetCondition() == ETaskCondition::Failed && !Objective->GetIsOptional())
		{
			return true;
		}
	}
	return false;
}

void UStage::OnRep_Condition()
{
	BroadcastChange_Condition();
}

void UStage::BroadcastChange_Condition()
{
	/*
	if(ConditionToDelegateMap.Contains(CurrentCondition))
	{
		FStageConditionDelegate DelegateToBroadcast = ConditionToDelegateMap[CurrentCondition];
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
