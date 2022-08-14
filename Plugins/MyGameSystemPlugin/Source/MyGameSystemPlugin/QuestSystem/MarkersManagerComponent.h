// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MarkersManagerComponent.generated.h"

/**
 * ActorComponent that manages markers for quests.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UMarkersManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	UMarkersManagerComponent();

	// EndPlay override.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * Marks actors for a quest.
	 * @param ActorsToMark - Objective actors for a quest.
	 */
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "MarkersManagerComponent")
	void MarkActors(const TArray<class AActor*>& ActorsToMark);

	/**
	 * Delete all set markers.
	 */
	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "MarkersManagerComponent")
	void DeleteAllMarkers();

	/**
	 * Marks actors for a quest on the current machine.
	 * @param ActorsToMark - Objective actors for a quest.
	 */
	UFUNCTION(BlueprintCallable, Category = "MarkersManagerComponent")
	void MarkActorsLocally(const TArray<class AActor*>& ActorsToMark);

	/**
	 * Delete all set markers on the current machine.
	 */
	UFUNCTION(BlueprintCallable, Category = "MarkersManagerComponent")
	void DeleteAllMarkersLocally();

protected:

	/**
	 * Class for marker objects.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MarkersManagerComponent", meta = (BlueprintProtected))
	class TSubclassOf<AActor> MarkerClass;

private:

	/**
	 * Array of currently instanced markers.
	 */
	UPROPERTY()
	TArray<class AActor*> Markers;

};
