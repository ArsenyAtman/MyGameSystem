// Fill out your copyright notice in the Description page of Project Settings.

#include "Objective.h"

#include "Stage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "QuestComponent.h"
#include "MarkersManagerComponent.h"
#include "ActorWithQuestsInterface.h"
#include "QuestActorsReferencer.h"
#include "Net/UnrealNetwork.h"

void UObjective::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UObjective, Condition);
	DOREPLIFETIME(UObjective, ReferencesForQuest);
}

void UObjective::Activate_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		ReferencesForQuest = FindReferencesForQuest();
		MarkersManager = CreateMarkersManager();
		SetCondition(ETaskCondition::InProcess);
		OnObjectiveActivated();
	}
}

void UObjective::Abort_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			EndObjective();
			SetCondition(ETaskCondition::Aborted);
		}
		OnObjectiveAborted();
	}
}

void UObjective::Mark()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if(IsValid(MarkersManager))
		{
			MarkersManager->MarkActors(FilterActorsForMarking(ReferencesForQuest.ActorsToMark));
		}
	}
}

void UObjective::Unmark()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if(IsValid(MarkersManager))
		{
			MarkersManager->DeleteAllMarkers();
		}
	}
}

UStage* UObjective::GetOwningStage() const
{
	return Cast<UStage>(GetOuter());
}

void UObjective::Update_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		Unmark();
		Mark();
		Notify_OnUpdated();
	}
}

void UObjective::EndObjective_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if(IsValid(MarkersManager))
		{
			MarkersManager->DestroyComponent();
		}
	}
}

void UObjective::SetCondition(ETaskCondition NewCondition)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		Condition = NewCondition;
		BroadcastChange_Condition();
	}
}

void UObjective::Complete_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			EndObjective();
			SetCondition(ETaskCondition::Completed);
			OnObjectiveCompleted();
			GetOwningStage()->ObjectiveCompleted(this);
		}
	}
}

void UObjective::Fail_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if (GetCondition() == ETaskCondition::InProcess)
		{
			EndObjective();
			SetCondition(ETaskCondition::Failed);
			OnObjectiveFailed();
			GetOwningStage()->ObjectiveFailed(this);
		}
	}
}

FReferencesForQuest UObjective::FindReferencesForQuest() const
{
	TArray<AActor*> FoundReferencers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestActorsReferencer::StaticClass(), FoundReferencers);

	for (AActor* ReferencerActor : FoundReferencers)
	{
		AQuestActorsReferencer* Referencer = Cast<AQuestActorsReferencer>(ReferencerActor);
		if (IsValid(Referencer))
		{
			return Referencer->GetReferencesForQuest(this->GetClass());
		}
	}

	return FReferencesForQuest();
}

UMarkersManagerComponent* UObjective::CreateMarkersManager() const
{
	AActor* QuestActor = GetOwningStage()->GetOwningQuest()->GetOwningQuestComponent()->GetOwner();
	if (IsValid(QuestActor))
	{
		return Cast<UMarkersManagerComponent>(QuestActor->AddComponentByClass(MarkersManagerComponentClass, false, FTransform::Identity, false));
	}

	return nullptr;
}

void UObjective::Notify_OnUpdated_Implementation()
{
	if(OnUpdated.IsBound())
	{
		OnUpdated.Broadcast(this);
	}
}

void UObjective::OnRep_Condition()
{
	BroadcastChange_Condition();
}

void UObjective::BroadcastChange_Condition()
{
	/*
	if(ConditionToDelegateMap.Contains(CurrentCondition))
	{
		FObjectiveConditionDelegate DelegateToBroadcast = ConditionToDelegateMap[CurrentCondition];
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

		case ETaskCondition::Aborted:
			OnAborted.Broadcast(this);
			break;
	}
}
