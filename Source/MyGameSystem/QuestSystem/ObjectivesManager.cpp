// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesManager.h"
#include "ObjectiveActorInterface.h"
#include "Objective.h"

// Sets default values
AObjectivesManager::AObjectivesManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectivesManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectivesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<AActor*> AObjectivesManager::GetOnLevelObjectives(UObjective* ForObjective)
{
	TArray<AActor*> OnLevelObjectives;
	for (FObjectivesOnLevel OnLevelObjective : Objectives)
	{
		if (OnLevelObjective.ObjectiveClass == ForObjective->GetClass())
		{
			for (AActor* RelatedObjective : OnLevelObjective.RelatedOnLevelObjectives)
			{
				if (RelatedObjective->Implements<UObjectiveActorInterface>())
				{
					OnLevelObjectives.Append(IObjectiveActorInterface::Execute_GetObjectives(RelatedObjective));
				}
				else
				{
					OnLevelObjectives.Add(RelatedObjective);
				}
			}
		}
	}
	return OnLevelObjectives;
}
