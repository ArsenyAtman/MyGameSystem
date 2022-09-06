// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedObject/ReplicatingActorComponent.h"
#include "EffectsComponent.generated.h"

/**
 * Delegate that handles events of an effect addition or removal.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatComponentEffectDelegate, class UEffect*, Effect);

/**
 * ActorComponent that handles effects.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEMPLUGIN_API UEffectsComponent : public UReplicatingActorComponent
{
	GENERATED_BODY()

public:

	UEffectsComponent();

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// TODO: Resolve naming of activation/deactivation methods.

	/**
	 * Apply a new effect.
	 * @param Effect - The new effect.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "EffectsComponent|Control")
	void ApplyEffect(class UEffect* Effect);

	/**
	 * Abort(stop/remove) an applied effect.
	 * @param Effect - The effect to remove.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "EffectsComponent|Control")
	void AbortEffect(class UEffect* Effect);

	/**
	 * Add an activated effect.
	 * @param Effect - The already activated effect.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "EffectsComponent|Internal")
	void AddEffect(class UEffect* Effect);

	/**
	 * Remove a deactivated effect.
	 * @param Effect - The already deactivated effect.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "EffectsComponent|Internal")
	void RemoveEffect(class UEffect* Effect);

	/**
	 * Get all applied effects to this component.
	 * @return List of effects.
	 */
	UFUNCTION(BlueprintGetter, Category = "EffectsComponent|Effects")
	TArray<class UEffect*> GetEffects() const { return Effects; }

	/**
	 * Get applied effects of a specified class.
	 * @param EffectClass - Class of effects.
	 * @return List of effects of the class.
	 */
	UFUNCTION(BlueprintPure, Category = "EffectsComponent|Effects")
	TArray<class UEffect*> GetEffectsOfClass(TSubclassOf<class UEffect> EffectClass) const;

	/**
	 * Called after an effect addition.
	 */
	UPROPERTY(BlueprintAssignable, Category = "EffectsComponent|Delegates")
	FStatComponentEffectDelegate OnEffectAdded;

	/**
	 * Called after an effect removal.
	 */
	UPROPERTY(BlueprintAssignable, Category = "EffectsComponent|Delegates")
	FStatComponentEffectDelegate OnEffectRemoved;

protected:

	// ...

private:

	/**
	 * All applied effects.
	 */
	UPROPERTY(BlueprintGetter = GetEffects, ReplicatedUsing = OnRep_Effects, Category = "EffectsComponent|Effects")
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
	
};
