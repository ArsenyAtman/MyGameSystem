// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedObject/AdvancedObject.h"
#include "StatsSystemTypes.h"
#include "Stat.generated.h"

/**
 * Delegate that handles events related to effects of a stat.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatEffectDelegate, class UStat*, Stat, class UEffect*, Effect);

/**
 * Delegate that handles events related to values of a stat.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatValueChangeDelegate, class UStat*, Stat, FStatValues, PrevValues);

/**
 * Object that handles values of some stat an impact of effects.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MYGAMESYSTEMPLUGIN_API UStat : public UAdvancedObject
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Apply delta to this stat.
	 * @param Delta - The delta to apply.
	 * @param Effect - The effect that applies the delta.
	 * @return The applied delta.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat|Internal")
	FStatValues ApplyDelta(FStatValues Delta, class UEffect* Effect);

	/**
	 * Add a new effect to this stat.
	 * @param Effect - The new effect.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat|Internal")
	void AddEffect(class UEffect* Effect);

	/**
	 * Remove an effect from this stat.
	 * @param Effect - The effect to remove.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Stat|Internal")
	void RemoveEffect(class UEffect* Effect);

	/**
	 * Get the values of this stat.
	 * @return The values of the stat.
	 */
	UFUNCTION(BlueprintGetter, Category = "Stat|Condition")
	FStatValues GetStatValues() const { return StatValues; }

	/**
	 * Get the base values of this stat.
	 * @return The base values of the stat.
	 */
	UFUNCTION(BlueprintGetter, Category = "Stat|Condition")
	FStatValues GetStatBaseValues() const { return StatBaseValues; }

	/**
	 * Get applied effects to this stat.
	 * @return Applied effects.
	 */
	UFUNCTION(BlueprintGetter, Category = "Stat|Effects")
	TArray<class UEffect*> GetEffects() const { return Effects; }

	/**
	 * Get the owning component of this stat.
	 * @return The owning stats component.
	 */
	UFUNCTION(BlueprintPure, Category = "Stat|StatsComponent")
	class UStatsComponent* GetOwningStatsComponent() const;

	/**
	 * Get the name of this stat.
	 * @return The name of this stat.
	 */
	UFUNCTION(BlueprintGetter, Category = "Stat|Name")
	FName GetStatName() const { return StatName; }

	/**
	 * Called after the values change.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatValueChangeDelegate OnValuesChanged;

	/**
	 * Called when the current value equals to the min value.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatValueChangeDelegate OnMinReached;

	/**
	 * Called when the current value equals to the max value.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatValueChangeDelegate OnMaxReached;

	/**
	 * Called after an effect addition.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatEffectDelegate OnEffectAdded;

	/**
	 * Called after an effect removal.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatEffectDelegate OnEffectRemoved;

protected:

	/**
	 * Called after the values changed.
	 * @param PrevValues - The previous values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat|Control", meta = (BlueprintProtected))
	void ValuesChanged(FStatValues PrevValues);
	virtual void ValuesChanged_Implementation(FStatValues PrevValues) { return; }

	/**
	 * Called when the current value equals to the min value.
	 * @param PrevValues - The previous values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat|Control", meta = (BlueprintProtected))
	void MinReached(FStatValues PrevValues);
	virtual void MinReached_Implementation(FStatValues PrevValues) { return; }

	/**
	 * Called when the current value equals to the max value.
	 * @param PrevValues - The previous values.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Stat|Control", meta = (BlueprintProtected))
	void MaxReached(FStatValues PrevValues);
	virtual void MaxReached_Implementation(FStatValues PrevValues) { return; }

	/**
	 * Set the stat values.
	 * @param NewStatValues - The new stat values.
	 */
	UFUNCTION(BlueprintSetter, Category = "Stat|Condition", meta = (BlueprintProtected))
	void SetStatValues(FStatValues NewStatValues);

	/**
	 * A name for indetification of this stat.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatName, Category = "Stat|Name", meta = (BlueprintProtected))
	FName StatName = "None";

private:

	/**
	 * Values of this stat.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatValues,  BlueprintSetter = SetStatValues, ReplicatedUsing = OnRep_StatValues, Category = "Stat|Condition", meta = (AllowPrivateAccess = true))
	FStatValues StatValues;

	// OnRep event for StatValues.
	UFUNCTION()
	void OnRep_StatValues(FStatValues PreReplicationStatValues);

	// Notify about the values change.
	void Changed_StatValues(FStatValues PrevValues);

	// Broadcast a related delegate.
	void Broadcast_OnValuesChanged(FStatValues PrevValues);
	void Broadcast_OnMinReached(FStatValues PrevValues);
	void Broadcast_OnMaxReached(FStatValues PrevValues);

	/**
	 * Base values of this stat.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatBaseValues, Category = "Stat|Condition", meta = (AllowPrivateAccess = true))
	FStatValues StatBaseValues;

	/**
	 * Applied effects to this stat.
	 */
	UPROPERTY(BlueprintGetter = GetEffects, ReplicatedUsing = OnRep_Effects, Category = "Stat|Effects")
	TArray<class UEffect*> Effects;

	// OnRep event for Effects.
	UFUNCTION()
	void OnRep_Effects(const TArray<class UEffect*>& PreReplicationEffects);

	// Broadcast a related delegate.
	void Broadcast_Effects(const TArray<class UEffect*>& PrevEffects);

	// Broadcast the delegate.
	void Broadcast_OnEffectAdded(class UEffect* Effect);

	// Broadcast the delegate.
	void Broadcast_OnEffectRemoved(class UEffect* Effect);

	/**
	 * DeltaApplier of this stat.
	 */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Stat|DeltaApplier", meta = (AllowPrivateAccess = true))
	class UStatDeltaApplier* StatDeltaApplier;

};
