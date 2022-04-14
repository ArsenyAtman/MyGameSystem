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
	Condition = ETaskCondition::InProcess;
	ReferencesForQuest = FindReferencesForQuest();
	MarkersManager = CreateMarkersManager();
	OnObjectiveActivated();
}

void UObjective::Abort_Implementation()
{
	if (Condition == ETaskCondition::InProcess)
	{
		Condition = ETaskCondition::Aborted;
		EndObjective();
	}
	OnObjectiveAborted();
}

void UObjective::Mark()
{
	if(IsValid(MarkersManager))
	{
		MarkersManager->MarkActors(FilterActorsForMarking(ReferencesForQuest.ActorsToMark));
	}
}

void UObjective::Unmark()
{
	if(IsValid(MarkersManager))
	{
		MarkersManager->DeleteAllMarkers();
	}
}

FObjectiveInfo UObjective::GetObjectiveInfo() const
{
	return FObjectiveInfo(ObjectiveData, Condition, Progress);
}

void UObjective::Update_Implementation()
{
	Unmark();
	Mark();
	OnObjectiveUpdated();
	Progress = RecalculateProgress();
	OwningStage->Update();
}

void UObjective::EndObjective_Implementation()
{
	if(IsValid(MarkersManager))
	{
		MarkersManager->DestroyComponent();
	}
}

void UObjective::Complete_Implementation()
{
	Condition = ETaskCondition::Completed;
	EndObjective();
	OnObjectiveCompleted();
	OwningStage->ObjectiveCompleted(this);
}

void UObjective::Fail_Implementation()
{
	Condition = ETaskCondition::Failed;
	EndObjective();
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

UMarkersManagerComponent* UObjective::CreateMarkersManager() const
{
	AActor* QuestActor = OwningStage->GetOwningQuest()->GetOwningQuestComponent()->GetOwner();
	if (IsValid(QuestActor))
	{
		return Cast<UMarkersManagerComponent>(QuestActor->AddComponentByClass(MarkersManagerComponentClass, false, FTransform::Identity, false));
	}

	return nullptr;
}