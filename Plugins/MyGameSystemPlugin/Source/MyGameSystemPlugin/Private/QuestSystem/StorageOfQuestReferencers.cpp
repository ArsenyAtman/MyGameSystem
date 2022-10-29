// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem/StorageOfQuestReferencers.h"

#include "QuestSystem/Objective.h"
#include "QuestSystem/QuestReferencer.h"

// Sets default values
AStorageOfQuestReferencers::AStorageOfQuestReferencers()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

UQuestReferencer* AStorageOfQuestReferencers::GetReferencesForQuest(TSubclassOf<UObjective> ObjectiveClass) const
{
	return ReferencesForQuests[ObjectiveClass];
}
