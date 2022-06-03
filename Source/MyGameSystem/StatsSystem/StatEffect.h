// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "StatEffect.generated.h"

/**
 * Effect that applies to a Stat.
 * @see UStat class.
 */
UCLASS()
class MYGAMESYSTEM_API UStatEffect : public UEffect
{
	GENERATED_BODY()

public:

	/**
	 * Getter for related stats of this effect.
	 * @return Related stats.
	 */
	UFUNCTION(BlueprintPure, Category = "StatEffect|RelatedStats")
	TArray<class UStat*> GetRelatedStats() const { return RelatedStats; }
	
protected:

	// Activation override.
	virtual void OnActivated_Implementation() override;

	// Deactivation override.
	virtual void OnDeactivating_Implementation() override;

	/**
	 * Class of stats affected by this effect.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatEffect|RelatedStats", meta = (ExposeOnSpawn = true, BlueprintProtected))
	TSubclassOf<class UStat> ForStatsOfClass;

private:

	// Stats that are related to this effect.
	TArray<class UStat*> RelatedStats;

};
