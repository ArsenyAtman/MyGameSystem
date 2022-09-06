// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectsComponent.h"
#include "StatsSystemTypes.h"
#include "StatsComponent.generated.h"

/**
 * Delegate that handles events of a stat addition or removal.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatComponentStatDelegate, class UStat*, Stat);

/**
 * ActorComponent that handles effects and stats.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UStatsComponent : public UEffectsComponent
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Add a new stat to this component.
	 * @param NewStat - The new stat to add.
	 * @return Addition result (false if failed to add).
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Stats")
	bool AddStat(class UStat* NewStat);

	/**
	 * Remove a stat from this component.
	 * @param NewStat - The stat to remove.
	 * @return Removal result (false if failed to remove).
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "StatsComponent|Stats")
	bool RemoveStat(class UStat* Stat);

	/**
	 * Get stats of this StatsComponent.
	 * @return List of stats.
	 */
	UFUNCTION(BlueprintGetter, Category = "StatComponent|Stats")
	TArray<class UStat*> GetStats() const { return Stats; }

	/**
	 * Get stats of a specified class.
	 * @param StatClass - Class of stats.
	 * @return List of stats of the class.
	 */
	UFUNCTION(BlueprintPure, Category = "StatComponent|Stats")
	TArray<class UStat*> GetStatsOfClass(TSubclassOf<class UStat> StatClass) const;

	/**
	 * Get stat of a specified name.
	 * @param StatName - The name of a stat.
	 * @return The first found stat with this name.
	 */
	UFUNCTION(BlueprintPure, Category = "StatComponent|Stats")
	class UStat* GetStatByName(FName StatName) const;

	/**
	 * Called after a stat addition.
	 */
	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatComponentStatDelegate OnStatAdded;

	/**
	 * Called after a stat removal.
	 */
	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatComponentStatDelegate OnStatRemoved;

protected:

	// ...

private:

	// TODO: Access stats via TMap.

	/**
	 * Stats of this component.
	 */
	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetStats, ReplicatedUsing = OnRep_Stats, Category = "StatComponent|Stats")
	TArray<class UStat*> Stats;

	// OnRep event of Stats
	UFUNCTION()
	void OnRep_Stats(const TArray<class UStat*>& PreReplicationStats);

	// Broadcast the delegate.
	void BroadcastChange_Stats(const TArray<class UStat*>& PrevStats);

	// Broadcast the delegate.
	void Broadcast_OnStatAdded(class UStat* Stat);

	// Broadcast the delegate.
	void Broadcast_OnStatRemoved(class UStat* Stat);

};
