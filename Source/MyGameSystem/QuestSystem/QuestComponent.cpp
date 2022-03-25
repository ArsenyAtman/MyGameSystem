// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "Quest.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ObjectiveActorInterface.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UQuestComponent, ActiveQuestsInfo, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(UQuestComponent, CompletedQuestsInfo, COND_OwnerOnly);

	DOREPLIFETIME_CONDITION(UQuestComponent, FailedQuestsInfo, COND_OwnerOnly);

}

void UQuestComponent::AddQuest(TSubclassOf<UQuest> QuestClass)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(QuestClass) && CheckQuestOnDuplication(QuestClass))
		{
			UQuest* NewQuest = NewObject<UQuest>(this, QuestClass);
			ActiveQuests.Add(NewQuest);
			NewQuest->Activate(this);
			UpdateInfos();
			QuestAddedMessage(NewQuest->GetQuestInfo());
		}
	}
}

void UQuestComponent::SelectQuestToTrack_Implementation(int QuestIndex)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (ActiveQuests.IsValidIndex(QuestIndex))
		{
			SetTrackedQuest(ActiveQuests[QuestIndex]);
		}
		else
		{
			SetTrackedQuest(nullptr);
		}
	}
}

void UQuestComponent::UntrackQuest_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		SetTrackedQuest(nullptr);
	}
}

void UQuestComponent::QuestCompleted(UQuest* Quest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Quest == TrackedQuest)
		{
			SetTrackedQuest(nullptr);
			TrackedQuestCompleted(Quest->GetQuestInfo());
		}

		if (ActiveQuests.Remove(Quest) > 0)
		{
			CompletedQuests.Add(Quest);
			UpdateInfos();
			QuestCompletedMessage(Quest->GetQuestInfo());
		}
	}
}

void UQuestComponent::QuestFailed(UQuest* Quest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Quest == TrackedQuest)
		{
			SetTrackedQuest(nullptr);
			TrackedQuestFailed(Quest->GetQuestInfo());
		}

		if (ActiveQuests.Remove(Quest) > 0)
		{
			FailedQuests.Add(Quest);
			UpdateInfos();
			QuestFailedMessage(Quest->GetQuestInfo());
		}
	}
}

void UQuestComponent::QuestUpdated(UQuest* Quest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Quest == TrackedQuest)
		{
			TrackedQuestUpdated(Quest->GetQuestInfo());
		}
		UpdateInfos();
	}
}

TArray<UQuest*> UQuestComponent::GetAllQuests()
{
	TArray<UQuest*> AllQuests = TArray<UQuest*>();
	AllQuests.Append(GetActiveQuests());
	AllQuests.Append(GetCompletedQuests());
	AllQuests.Append(GetFailedQuests());
	return AllQuests;
}

void UQuestComponent::MarkActors_Implementation(TSubclassOf<AActor> MarkerClass, const TArray<AActor*>& ActorsToMark)
{
	for (AActor* ActorToMark : ActorsToMark)
	{
		AActor* NewMarker = GetWorld()->SpawnActor<AActor>(MarkerClass, FTransform());
		if (IsValid(NewMarker))
		{
			NewMarker->AttachToActor(ActorToMark, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
			if (ActorToMark->Implements<UObjectiveActorInterface>())
			{
				FTransform MarkerTransfrom = IObjectiveActorInterface::Execute_GetMarkerRelativeTransform(ActorToMark);
				NewMarker->SetActorRelativeTransform(MarkerTransfrom);
			}
			Markers.Add(NewMarker);
		}
	}
}

void UQuestComponent::UnmarkActors_Implementation(TSubclassOf<AActor> MarkerClass, const TArray<AActor*>& ActorsToUnmark)
{
	for (int i = Markers.Num() - 1; i >= 0; --i)
	{
		auto Marker = Markers[i];
		if (IsValid(Marker) && (!IsValid(Marker->GetAttachParentActor()) || (Marker->GetClass() == MarkerClass && ActorsToUnmark.Find(Marker->GetAttachParentActor()) != INDEX_NONE)))
		{
			Marker->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
			Marker->Destroy();
		}
		Markers.Remove(Marker);
	}
}

void UQuestComponent::UpdateInfos()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		ActiveQuestsInfo.Empty();
		ActiveQuestsInfo = GetQuestsInfo(ActiveQuests);

		CompletedQuestsInfo.Empty();
		CompletedQuestsInfo = GetQuestsInfo(CompletedQuests);

		FailedQuestsInfo.Empty();
		FailedQuestsInfo = GetQuestsInfo(FailedQuests);

		OnUpdated.Broadcast();
	}
}

TArray<FQuestInfo> UQuestComponent::GetQuestsInfo(TArray<UQuest*> Quests)
{
	TArray<FQuestInfo> QuestsInfo;
	for (UQuest* Quest : Quests)
	{
		QuestsInfo.Add(Quest->GetQuestInfo());
	}
	return QuestsInfo;
}

bool UQuestComponent::CheckQuestOnDuplication(TSubclassOf<UQuest> QuestClass)
{
	TArray<UQuest*> AllQuests = GetAllQuests();
	for (UQuest* QuestObject : AllQuests)
	{
		if (QuestObject->GetClass() == QuestClass)
		{
			return false;
		}
	}
	return true;
}

void UQuestComponent::SetTrackedQuest(UQuest* NewTrackedQuest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(TrackedQuest))
		{
			TrackedQuest->SetIsBeingTracked(false);
		}
		TrackedQuest = NewTrackedQuest;
		FQuestInfo QuestInfo;
		if (IsValid(TrackedQuest))
		{
			TrackedQuest->SetIsBeingTracked(true);
			QuestInfo = TrackedQuest->GetQuestInfo();
		}
		UpdateInfos();
		TrackedQuestSelected(QuestInfo);
	}
}

void UQuestComponent::TrackedQuestSelected_Implementation(FQuestInfo NewTrackedQuestInfo)
{
	TrackedQuestInfo = NewTrackedQuestInfo;
	OnTrackedQuestSelected.Broadcast();
}

void UQuestComponent::TrackedQuestUpdated_Implementation(FQuestInfo NewTrackedQuestInfo)
{
	TrackedQuestInfo = NewTrackedQuestInfo;
	OnTrackedQuestUpdated.Broadcast();
}

void UQuestComponent::TrackedQuestCompleted_Implementation(FQuestInfo QuestInfo)
{
	OnTrackedQuestCompleted.Broadcast(QuestInfo);
}

void UQuestComponent::TrackedQuestFailed_Implementation(FQuestInfo QuestInfo)
{
	OnTrackedQuestFailed.Broadcast(QuestInfo);
}

void UQuestComponent::QuestAddedMessage_Implementation(FQuestInfo QuestInfo)
{
	OnQuestAdded.Broadcast(QuestInfo);
}

void UQuestComponent::QuestCompletedMessage_Implementation(FQuestInfo QuestInfo)
{
	OnQuestCompleted.Broadcast(QuestInfo);
}

void UQuestComponent::QuestFailedMessage_Implementation(FQuestInfo QuestInfo)
{
	OnQuestFailed.Broadcast(QuestInfo);
}

void UQuestComponent::OnRep_ActiveQuestsInfo()
{
	OnUpdated.Broadcast();
}

void UQuestComponent::OnRep_CompletedQuestsInfo()
{
	OnUpdated.Broadcast();
}

void UQuestComponent::OnRep_FailedQuestsInfo()
{
	OnUpdated.Broadcast();
}
