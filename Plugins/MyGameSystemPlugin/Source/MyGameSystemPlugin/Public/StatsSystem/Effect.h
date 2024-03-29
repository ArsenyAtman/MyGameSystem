// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedObject/AdvancedObject.h"
#include "StatsSystemTypes.h"
#include "Effect.generated.h"

/**
 * Delegate for handling changes of an effect object.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEffectConditionChangeDelegate, class UEffect*, Effect);

/**
 * Effect that can be applied to a stats component and affect it or it's actor.
 * @see UStatsComponent class.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UEffect : public UAdvancedObject
{
	GENERATED_BODY()

public:

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Activate this effect
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect|Internal")
	void Activate();

	/**
	 * Deactivate this effect
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect|Control")
	void Deactivate();

	/**
	 * Gets the EffectsComponent related to this effect.
	 * @return Related effects component.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect|RelatedEffectsComponent")
	class UEffectsComponent* GetRelatedEffectsComponent() const;

	/**
	 * Gets the actor related to this effect.
	 * @return The controlled actor by this effect.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect|RelatedActor")
	class AActor* GetRelatedActor() const;

	/**
	 * Called after the activation.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Effect|Delegates")
	FEffectConditionChangeDelegate OnEffectActivated;

	/**
	 * Called after the deactivation.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Effect|Delegates")
	FEffectConditionChangeDelegate OnEffectDeactivating;

protected:

	/**
	 * Called after the activation.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control", meta = (BlueprintProtected))
	void OnActivated();
	virtual void OnActivated_Implementation() { return; }

	/**
	 * Called before the destruction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control", meta = (BlueprintProtected))
	void OnDeactivating();
	virtual void OnDeactivating_Implementation() { return; }

private:

	// Notify all clients of the activation.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_OnActivated();

	UFUNCTION(NetMulticast, Reliable)
	void Notify_OnDeactivating();

	// Call OnActivated delegate.
	void Broadcast_OnActivated();

	// Call OnDeactivated delegate.
	void Broadcast_OnDeactivating();
	
};
