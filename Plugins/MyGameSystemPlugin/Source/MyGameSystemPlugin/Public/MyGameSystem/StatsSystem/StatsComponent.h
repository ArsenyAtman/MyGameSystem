// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedObject/ReplicatingActorComponent.h"
#include "StatsSystemTypes.h"
#include "StatsComponent.generated.h"

/**
 * Delegate that handles events of an effect addition or removal.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatComponentEffectDelegate, class UEffect*, Effect);

/**
 * Delegate that handles events of a stat addition or removal.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatComponentStatDelegate, class UStat*, Stat);

// TODO: Create a base class named EffectsComponent.

/**
 * ActorComponent that handles effects and stats.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UStatsComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// TODO: Resolve naming of activation/deactivation methods.

	/**
	 * Apply a new effect.
	 * @param Effect - The new effect.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "StatComponent|Control")
	void ApplyEffect(class UEffect* Effect);

	/**
	 * Abort(stop/remove) an applied effect.
	 * @param Effect - The effect to remove.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "StatComponent|Control")
	void AbortEffect(class UEffect* Effect);

	/**
	 * Add an activated effect.
	 * @param Effect - The already activated effect.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "StatComponent|Internal")
	void AddEffect(class UEffect* Effect);

	/**
	 * Remove a deactivated effect.
	 * @param Effect - The already deactivated effect.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "StatComponent|Internal")
	void RemoveEffect(class UEffect* Effect);

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
	 * Get all applied effects to this component.
	 * @return List of effects.
	 */
	UFUNCTION(BlueprintGetter, Category = "StatComponent|Effects")
	TArray<class UEffect*> GetEffects() const { return Effects; }

	/**
	 * Get applied effects of a specified class.
	 * @param EffectClass - Class of effects.
	 * @return List of effects of the class.
	 */
	UFUNCTION(BlueprintPure, Category = "StatComponent|Effects")
	TArray<class UEffect*> GetEffectsOfClass(TSubclassOf<class UEffect> EffectClass) const;

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
	 * Called after an effect addition.
	 */
	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatComponentEffectDelegate OnEffectAdded;

	/**
	 * Called after an effect removal.
	 */
	UPROPERTY(BlueprintAssignable, Category = "StatComponent|Delegates")
	FStatComponentEffectDelegate OnEffectRemoved;

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

	/**
	 * All applied effects.
	 */
	UPROPERTY(BlueprintGetter = GetEffects, ReplicatedUsing = OnRep_Effects, Category = "StatComponent|Effects")
	TArray<class UEffect*> Effects;

	// OnRep event of Effects
	UFUNCTION()
	void OnRep_Effects(const TArray<class UEffect*>& PreReplicationEffects);

	// Broadcast the delegate.
	void BroadcastChange_Effects(const TArray<class UEffect*>& PrevEffects);

	// Broadcast the delegate.
	void Broadcast_OnEffectAdded(class UEffect* Effect);

	// Broadcast the delegate.
	void Broadcast_OnEffectRemoved(class UEffect* Effect);

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
