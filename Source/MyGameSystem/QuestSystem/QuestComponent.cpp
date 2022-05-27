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

	DOREPLIFETIME_CONDITION(UQuestComponent, ActiveQuests, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UQuestComponent, CompletedQuests, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UQuestComponent, FailedQuests, COND_OwnerOnly);

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
			NewQuest->Activate();
			BroadcastChange_QuestAdded(NewQuest);
		}
	}
}

void UQuestComponent::QuestCompleted(UQuest* Quest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (ActiveQuests.Remove(Quest) > 0)
		{
			CompletedQuests.Add(Quest);
			BroadcastChange_QuestCompleted(Quest);
		}
	}
}

void UQuestComponent::QuestFailed(UQuest* Quest)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (ActiveQuests.Remove(Quest) > 0)
		{
			FailedQuests.Add(Quest);
			BroadcastChange_QuestFailed(Quest);
		}
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

void UQuestComponent::OnRep_ActiveQuests(const TArray<UQuest*>& PreReplicationActiveQuests)
{
	PerformeFunctionForArrayDiff(PreReplicationActiveQuests, ActiveQuests, &UQuestComponent::BroadcastChange_QuestAdded);
}

void UQuestComponent::OnRep_CompletedQuests(const TArray<UQuest*>& PreReplicationCompletedQuests)
{
	PerformeFunctionForArrayDiff(PreReplicationCompletedQuests, CompletedQuests, &UQuestComponent::BroadcastChange_QuestCompleted);
}

void UQuestComponent::OnRep_FailedQuests(const TArray<UQuest*>& PreReplicationFailedQuests)
{
	PerformeFunctionForArrayDiff(PreReplicationFailedQuests, FailedQuests, &UQuestComponent::BroadcastChange_QuestFailed);
}

void UQuestComponent::BroadcastChange_QuestAdded(UQuest* NewQuest)
{
	OnQuestAdded.Broadcast(NewQuest);
}

void UQuestComponent::BroadcastChange_QuestCompleted(UQuest* NewQuest)
{
	OnQuestCompleted.Broadcast(NewQuest);
}

void UQuestComponent::BroadcastChange_QuestFailed(UQuest* NewQuest)
{
	OnQuestFailed.Broadcast(NewQuest);
}

void UQuestComponent::PerformeFunctionForArrayDiff(const TArray<UQuest*>& ArrayBefore, const TArray<UQuest*>& ArrayAfter, BroadcastChangeFunction Function)
{
	for (int i = 0; i < ArrayAfter.Num(); ++i)
	{
		if(i >= ArrayBefore.Num() || ArrayAfter[i] != ArrayBefore[i])
		{
			((*this).*Function)(ArrayAfter[i]);
		}
	}
	/*
	int SizeBefore = ArrayBefore.Num();
	int SizeAfter = ArrayAfter.Num();
	int SizeDifference = SizeAfter - SizeBefore;
	if(SizeDifference > 0)
	{
		for (int i = SizeBefore; i < SizeAfter; i++)
		{
			((*this).*Function)(ArrayAfter[i]);
		}
	}
	*/
}
