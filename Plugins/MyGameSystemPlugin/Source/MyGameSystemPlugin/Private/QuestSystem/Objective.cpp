// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestSystem/Objective.h"

#include "QuestSystem/Stage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "QuestSystem/QuestComponent.h"
#include "QuestSystem/MarkersManagerComponent.h"
#include "QuestSystem/ActorWithQuestsInterface.h"
#include "QuestSystem/StorageOfQuestReferencers.h"
#include "QuestSystem/QuestReferencer.h"
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
	}
}

void UObjective::Mark()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		if(IsValid(MarkersManager))
		{
			MarkersManager->MarkActors(GetActorsForMarking());
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
		Notify_OnUpdated();
		Unmark();
		Mark();
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
			GetOwningStage()->ObjectiveFailed(this);
		}
	}
}

UQuestReferencer* UObjective::FindReferencesForQuest() const
{
	TArray<AActor*> FoundReferencers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStorageOfQuestReferencers::StaticClass(), FoundReferencers);

	for (AActor* ReferencerActor : FoundReferencers)
	{
		AStorageOfQuestReferencers* StorageOfReferencers = Cast<AStorageOfQuestReferencers>(ReferencerActor);
		if (IsValid(StorageOfReferencers))
		{
			return StorageOfReferencers->GetReferencesForQuest(this->GetClass());
		}
	}

	return nullptr;
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
			OnObjectiveActivated();
			OnActivated.Broadcast(this);
			break;

		case ETaskCondition::Completed:
			OnObjectiveCompleted();
			OnCompleted.Broadcast(this);
			break;

		case ETaskCondition::Failed:
			OnObjectiveFailed();
			OnFailed.Broadcast(this);
			break;

		case ETaskCondition::Aborted:
			OnObjectiveAborted();
			OnAborted.Broadcast(this);
			break;
	}
}

void UObjective::Notify_OnUpdated_Implementation()
{
	OnUpdated.Broadcast(this);
}
