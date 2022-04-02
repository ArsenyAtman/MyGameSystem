// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestSystemTypes.h"
#include "QuestActorsReferencer.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AQuestActorsReferencer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestActorsReferencer();

	UFUNCTION(BlueprintPure)
	FReferencesForQuest GetReferencesForQuest(TSubclassOf<class UObjective> ObjectiveClass);

private:

	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<class UObjective>, FReferencesForQuest> ReferencesForQuests;
};
