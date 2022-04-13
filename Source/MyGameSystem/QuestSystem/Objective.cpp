// Fill out your copyright notice in the Description page of Project Settings.

#include "Objective.h"

#include "Stage.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "QuestComponent.h"
#include "MarkersManagerComponent.h"
#include "ActorWithQuestsInterface.h"
#include "QuestActorsReferencer.h"

void UObjective::Activate_Implementation(UStage* RelatedStage)
{
	OwningStage = RelatedStage;
	ObjectiveInfo.Condition = ETaskCondition::InProcess;
	ReferencesForQuest = FindReferencesForQuest();
	OnObjectiveActivated();
}

void UObjective::Abort_Implementation()
{
	if (ObjectiveInfo.Condition == ETaskCondition::InProcess)
	{
		ObjectiveInfo.Condition = ETaskCondition::Aborted;
		Unmark();
	}
	OnObjectiveAborted();
}

void UObjective::Mark()
{
	AActor* QuestActor = OwningStage->GetOwningQuest()->GetOwningQuestComponent()->GetOwner();
	if (IsValid(QuestActor) && QuestActor->Implements<UActorWithQuestsInterface>())
	{
		UMarkersManagerComponent* MarkersManager = IActorWithQuestsInterface::Execute_GetActorMarkersManagerComponent(QuestActor);
		if(IsValid(MarkersManager))
		{
			Markers = MarkersManager->MarkActorsReplicated(MarkerClass, FilterActorsForMarking(ReferencesForQuest.ActorsToMark));
		}
	}
}

void UObjective::Unmark()
{
	AActor* QuestActor = OwningStage->GetOwningQuest()->GetOwningQuestComponent()->GetOwner();
	if (IsValid(QuestActor) && QuestActor->Implements<UActorWithQuestsInterface>())
	{
		UMarkersManagerComponent* MarkersManager = IActorWithQuestsInterface::Execute_GetActorMarkersManagerComponent(QuestActor);
		if(IsValid(MarkersManager))
		{
			MarkersManager->UnmarkActors(Markers);
			Markers.Empty();
		}
	}
}

void UObjective::Update_Implementation()
{
	Unmark();
	Mark();
	OnObjectiveUpdated();
	OwningStage->Update();
}

void UObjective::Complete_Implementation()
{
	ObjectiveInfo.Condition = ETaskCondition::Completed;
	Unmark();
	OnObjectiveCompleted();
	OwningStage->ObjectiveCompleted(this);
}

void UObjective::Fail_Implementation()
{
	ObjectiveInfo.Condition = ETaskCondition::Failed;
	Unmark();
	OnObjectiveFailed();
	OwningStage->ObjectiveFailed(this);
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
