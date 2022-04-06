// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "StatEffect.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UStatEffect : public UEffect
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "StatEffect|RelatedStats")
	TArray<class UStat*> GetRelatedStats() { return RelatedStats; }
	
protected:

	virtual void OnActivated_Implementation();

	virtual void OnDeactivating_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "StatEffect|RelatedStats")
	TSubclassOf<class UStat> ForStatsOfClass;

private:

	TArray<class UStat*> RelatedStats;

};
