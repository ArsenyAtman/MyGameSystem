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

	ActorsForQuest = FindActorsForQuest();
	ActorsForMarking = FindActorsForMarking();
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
			Markers = MarkersManager->MarkActorsReplicated(MarkerClass, FilterActorsForMarking(ActorsForMarking));
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

TArray<AActor*> UObjective::FindActorsForQuest()
{
	TArray<AActor*> FoundActors;

	TArray<AActor*> FoundReferencers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestActorsReferencer::StaticClass(), FoundReferencers);

	for (AActor* ReferencerActor : FoundReferencers)
	{
		AQuestActorsReferencer* Referencer = Cast<AQuestActorsReferencer>(ReferencerActor);
		if (Referencer->GetObjectiveClass() == this->StaticClass())
		{
			FoundActors.Append(Referencer->GetRelatedActors());
		}
	}

	return FoundActors;
}

TArray<AActor*> UObjective::FindActorsForMarking()
{
	TArray<AActor*> FoundActors;

	TArray<AActor*> FoundReferencers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuestActorsReferencer::StaticClass(), FoundReferencers);

	for (AActor* ReferencerActor : FoundReferencers)
	{
		AQuestActorsReferencer* Referencer = Cast<AQuestActorsReferencer>(ReferencerActor);
		if (Referencer->GetObjectiveClass() == this->StaticClass())
		{
			FoundActors.Append(Referencer->GetActorsToMark());
		}
	}

	return FoundActors;
}
