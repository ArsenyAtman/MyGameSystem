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

FReferencesForQuest UObjective::FindReferencesForQuest()
{
	TArray<AActor*> FoundReferencers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestActorsReferencer::StaticClass(), FoundReferencers);

	for (AActor* ReferencerActor : FoundReferencers)
	{
		AQuestActorsReferencer* Referencer = Cast<AQuestActorsReferencer>(ReferencerActor);
		if (IsValid(Referencer))
		{
			return Referencer->GetReferencesForQuest(this->StaticClass());
		}
	}

	return FReferencesForQuest();
}
