// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "Quest.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
	
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UQuestComponent, ActiveQuestsInfo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UQuestComponent, CompletedQuestsInfo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UQuestComponent, FailedQuestsInfo, COND_OwnerOnly);

}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
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
			UpdateAllQuestsInfo();
			QuestAddedNotify(NewQuest->GetQuestInfo());
		}
	}
}

void UQuestComponent::SelectQuestToTrackByIndex_Implementation(int QuestIndex)
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

void UQuestComponent::SelectQuestToTrackByInfo_Implementation(FQuestInfo Info)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		int QuestIndex = ActiveQuestsInfo.Find(Info);
		if (QuestIndex != INDEX_NONE)
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
			TrackedQuestCompletedNotify(Quest->GetQuestInfo());
		}

		if (ActiveQuests.Remove(Quest) > 0)
		{
			CompletedQuests.Add(Quest);
			UpdateAllQuestsInfo();
			QuestCompletedNotify(Quest->GetQuestInfo());
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
			TrackedQuestFailedNotify(Quest->GetQuestInfo());
		}

		if (ActiveQuests.Remove(Quest) > 0)
		{
			FailedQuests.Add(Quest);
			UpdateAllQuestsInfo();
			QuestFailedNotify(Quest->GetQuestInfo());
		}
	}
}

void UQuestComponent::QuestUpdated(UQuest* Quest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (Quest == TrackedQuest)
		{
			TrackedQuestUpdatedNotify(Quest->GetQuestInfo());
		}
		UpdateAllQuestsInfo();
	}
}

TArray<UQuest*> UQuestComponent::GetAllQuests() const
{
	TArray<UQuest*> AllQuests = TArray<UQuest*>();
	AllQuests.Append(GetActiveQuests());
	AllQuests.Append(GetCompletedQuests());
	AllQuests.Append(GetFailedQuests());
	return AllQuests;
}

bool UQuestComponent::HasTrackedQuest() const
{
	return TrackedQuestInfo != FQuestInfo();
}

void UQuestComponent::UpdateAllQuestsInfo()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		ActiveQuestsInfo.Empty();
		ActiveQuestsInfo = GetQuestsInfo(ActiveQuests);

		CompletedQuestsInfo.Empty();
		CompletedQuestsInfo = GetQuestsInfo(CompletedQuests);

		FailedQuestsInfo.Empty();
		FailedQuestsInfo = GetQuestsInfo(FailedQuests);

		OnQuestsUpdated.Broadcast();
	}
}

TArray<FQuestInfo> UQuestComponent::GetQuestsInfo(const TArray<UQuest*>& Quests) const
{
	TArray<FQuestInfo> QuestsInfo;
	for (const UQuest* Quest : Quests)
	{
		QuestsInfo.Add(Quest->GetQuestInfo());
	}
	return QuestsInfo;
}

bool UQuestComponent::CheckQuestOnDuplication(TSubclassOf<UQuest> QuestClass) const
{
	TArray<UQuest*> AllQuests = GetAllQuests();
	for (const UQuest* QuestObject : AllQuests)
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

		if (IsValid(TrackedQuest))
		{
			TrackedQuest->SetIsBeingTracked(true);
			FQuestInfo QuestInfo = TrackedQuest->GetQuestInfo();
			UpdateAllQuestsInfo();
			TrackedQuestSelectedNotify(QuestInfo);
		}
		else
		{
			TrackedQuestDeselectedNotify();
		}
		
	}
}

void UQuestComponent::TrackedQuestSelectedNotify_Implementation(FQuestInfo NewTrackedQuestInfo)
{
	TrackedQuestInfo = NewTrackedQuestInfo;
	OnTrackedQuestSelected.Broadcast();
}

void UQuestComponent::TrackedQuestDeselectedNotify_Implementation()
{
	TrackedQuestInfo = FQuestInfo();
	OnTrackedQuestDeselected.Broadcast();
}

void UQuestComponent::TrackedQuestUpdatedNotify_Implementation(FQuestInfo NewTrackedQuestInfo)
{
	TrackedQuestInfo = NewTrackedQuestInfo;
	OnTrackedQuestUpdated.Broadcast();
}

void UQuestComponent::TrackedQuestCompletedNotify_Implementation(FQuestInfo QuestInfo)
{
	TrackedQuestInfo = FQuestInfo();
	OnTrackedQuestCompleted.Broadcast(QuestInfo);
}

void UQuestComponent::TrackedQuestFailedNotify_Implementation(FQuestInfo QuestInfo)
{
	TrackedQuestInfo = FQuestInfo();
	OnTrackedQuestFailed.Broadcast(QuestInfo);
}

void UQuestComponent::QuestAddedNotify_Implementation(FQuestInfo QuestInfo)
{
	OnQuestAdded.Broadcast(QuestInfo);
}

void UQuestComponent::QuestCompletedNotify_Implementation(FQuestInfo QuestInfo)
{
	OnQuestCompleted.Broadcast(QuestInfo);
}

void UQuestComponent::QuestFailedNotify_Implementation(FQuestInfo QuestInfo)
{
	OnQuestFailed.Broadcast(QuestInfo);
}

void UQuestComponent::OnRep_ActiveQuestsInfo()
{
	OnQuestsUpdated.Broadcast();
}

void UQuestComponent::OnRep_CompletedQuestsInfo()
{
	OnQuestsUpdated.Broadcast();
}

void UQuestComponent::OnRep_FailedQuestsInfo()
{
	OnQuestsUpdated.Broadcast();
}
