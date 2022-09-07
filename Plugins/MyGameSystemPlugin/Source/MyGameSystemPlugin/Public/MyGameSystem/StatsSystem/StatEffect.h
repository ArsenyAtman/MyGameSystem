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
class MYGAMESYSTEMPLUGIN_API UStatEffect : public UEffect
{
	GENERATED_BODY()

public:

	/**
	 * Getter for related stats of this effect.
	 * @return Related stats.
	 */
	UFUNCTION(BlueprintGetter, Category = "StatEffect|RelatedStats")
	TArray<class UStat*> GetRelatedStats() const { return RelatedStats; }

	/**
	 * Gets the StatsComponent related to this effect.
	 * @return Related stats component.
	 */
	UFUNCTION(BlueprintPure, Category = "StatEffect|RelatedStatsComponent")
	class UStatsComponent* GetRelatedStatsComponent() const;
	
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

	/**
	 * Related stats to this effect.
	 */
	UPROPERTY(BlueprintGetter = GetRelatedStats)
	TArray<UStat*> RelatedStats;

	// Called when a stat added to the owning stats component.
	UFUNCTION()
	void OnStatAdded(class UStat* Stat);

	// Called when a stat removed from the owning stats component.
	UFUNCTION()
	void OnStatRemoved(class UStat* Stat);

};
