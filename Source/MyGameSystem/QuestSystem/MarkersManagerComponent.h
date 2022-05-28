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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "MarkersManagerComponent")
	void MarkActors(const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "MarkersManagerComponent")
	void DeleteAllMarkers();

	UFUNCTION(BlueprintCallable, Category = "MarkersManagerComponent")
	void MarkActorsLocally(const TArray<class AActor*>& ActorsToMark);

	UFUNCTION(BlueprintCallable, Category = "MarkersManagerComponent")
	void DeleteAllMarkersLocally();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MarkersManagerComponent", meta = (BlueprintProtected))
	class TSubclassOf<AActor> MarkerClass;

private:

	UPROPERTY()
	TArray<class AActor*> Markers;

};
