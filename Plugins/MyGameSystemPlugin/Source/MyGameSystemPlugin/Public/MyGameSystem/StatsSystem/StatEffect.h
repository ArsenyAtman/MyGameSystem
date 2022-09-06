// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect.h"
#include "StatEffect.generated.h"

UENUM(BlueprintType)
enum class ERelatedStatsSearchType : uint8
{
	SearchByNameOnly			UMETA(DisplayName = "SearchByNameOnly"),
	SearchByClassOnly			UMETA(DisplayName = "SearchByClassOnly"),
	SearchByNameAndByClass		UMETA(DisplayName = "SearchByNameAndByClass")
};

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
	UFUNCTION(BlueprintPure, Category = "StatEffect|RelatedStats")
	TArray<class UStat*> GetRelatedStats() const;

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
	 * Define the search method for related stats.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatEffect|RelatedStats", meta = (ExposeOnSpawn = true, BlueprintProtected))
	ERelatedStatsSearchType RelatedStatsSearchType = ERelatedStatsSearchType::SearchByNameAndByClass;

	/**
	 * Class of stats affected by this effect.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatEffect|RelatedStats", meta = (ExposeOnSpawn = true, BlueprintProtected))
	TSubclassOf<class UStat> ForStatsOfClass;

	/**
	 * Name of a stat affected by this effect.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StatEffect|RelatedStats", meta = (ExposeOnSpawn = true, BlueprintProtected))
	FName ForStatOfName;

private:

	// TODO: Remove duplicating stats.

	// Add related stats to the provided array.
	void AddRelatedStatByName(TArray<class UStat*>& RelatedStats, FName Name) const;
	void AddRelatedStatsByClass(TArray<class UStat*>& RelatedStats, TSubclassOf<class UStat> Class) const;

};
