// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSystemTypes.h"
#include "QuestActorsReferencer.generated.h"

/**
 * Actor to refer actors for quests from a level.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AQuestActorsReferencer : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AQuestActorsReferencer();

	/**
	 * Get actors of an objective from a level.
	 * @param ObjectiveClass - Class of the objective.
	 * @return Referenced actors.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestActorsReferencer")
	FReferencesForQuest GetReferencesForQuest(TSubclassOf<class UObjective> ObjectiveClass) const;

protected:

	// ...

private:

	/**
	 * Map for binding actors and quests.
	 */
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<class UObjective>, FReferencesForQuest> ReferencesForQuests;
};
