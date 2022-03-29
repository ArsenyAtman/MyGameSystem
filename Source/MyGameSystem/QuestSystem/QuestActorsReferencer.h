// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestActorsReferencer.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AQuestActorsReferencer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestActorsReferencer();

	UFUNCTION(BlueprintGetter)
	TSubclassOf<class UObjective> GetObjectiveClass() { return ObjectiveClass; }

	UFUNCTION(BlueprintGetter)
	TArray<class AActor*> GetRelatedActors() { return RelatedActors; }

private:

	UPROPERTY(EditAnywhere, BlueprintGetter = GetObjectiveClass, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	TSubclassOf<class UObjective> ObjectiveClass;

	UPROPERTY(EditAnywhere, BlueprintGetter = GetRelatedActors, meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	TArray<class AActor*> RelatedActors;
};
