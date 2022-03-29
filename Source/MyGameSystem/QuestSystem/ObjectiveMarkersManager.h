// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestActorsReferencer.h"
#include "ObjectiveMarkersManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AObjectiveMarkersManager : public AQuestActorsReferencer
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveMarkersManager();
};
