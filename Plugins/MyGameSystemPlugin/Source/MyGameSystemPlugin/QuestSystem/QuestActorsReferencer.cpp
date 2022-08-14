// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestActorsReferencer.h"
#include "Objective.h"

// Sets default values
AQuestActorsReferencer::AQuestActorsReferencer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

FReferencesForQuest AQuestActorsReferencer::GetReferencesForQuest(TSubclassOf<UObjective> ObjectiveClass) const
{
	return ReferencesForQuests[ObjectiveClass];
}

