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
			NewQuest->Activate(this);
			QuestAddedNotify(NewQuest);
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
			QuestCompletedNotify(Quest);
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
			QuestFailedNotify(Quest);
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

void UQuestComponent::QuestAddedNotify_Implementation(UQuest* Quest)
{
	OnQuestAdded.Broadcast(Quest);
}

void UQuestComponent::QuestCompletedNotify_Implementation(UQuest* Quest)
{
	OnQuestCompleted.Broadcast(Quest);
}

void UQuestComponent::QuestFailedNotify_Implementation(UQuest* Quest)
{
	OnQuestFailed.Broadcast(Quest);
}

void UQuestComponent::OnRep_ActiveQuests()
{
	OnActiveQuestsUpdated.Broadcast();
}

void UQuestComponent::OnRep_CompletedQuests()
{
	OnCompletedQuestsUpdated.Broadcast();
}

void UQuestComponent::OnRep_FailedQuests()
{
	OnFailedQuestsUpdated.Broadcast();
}
