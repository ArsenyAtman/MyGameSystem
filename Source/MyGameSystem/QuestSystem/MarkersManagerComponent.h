// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MarkersManagerComponent.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UMarkersManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UMarkersManagerComponent();

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> MarkActorsReplicated(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(BlueprintCallable)
	TArray<class AActor*> MarkActorsLocally(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void MarkActorsOnClients(TSubclassOf<class AActor> MarkerClass, const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(BlueprintCallable)
	void UnmarkActors(const TArray<class AActor*>& Markers);

};
