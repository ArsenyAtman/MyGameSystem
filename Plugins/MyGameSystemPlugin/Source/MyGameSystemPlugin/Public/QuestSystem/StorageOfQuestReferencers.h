// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StorageOfQuestReferencers.generated.h"

class UObjective;
class UQuestReferencer;

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API AStorageOfQuestReferencers : public AActor
{
	GENERATED_BODY()
	
public:

	// Sets default values for this actor's properties
	AStorageOfQuestReferencers();

	/**
	 * Get actors of an objective from a level.
	 * @param ObjectiveClass - Class of the objective.
	 * @return Referenced actors.
	 */
	UFUNCTION(BlueprintPure, Category = "QuestActorsReferencer")
	UQuestReferencer* GetReferencesForQuest(TSubclassOf<UObjective> ObjectiveClass) const;

protected:

	// ...

private:

	/**
	 * Map for binding references and quests.
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TMap<TSubclassOf<class UObjective>, UQuestReferencer*> ReferencesForQuests;

};
