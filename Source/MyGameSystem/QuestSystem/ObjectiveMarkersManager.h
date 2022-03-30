// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveMarkersManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API AObjectiveMarkersManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectiveMarkersManager();

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> MarkActors(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> UnmarkActors(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToUnmark);

private:

	TArray<class AActor*> Markers;

};
