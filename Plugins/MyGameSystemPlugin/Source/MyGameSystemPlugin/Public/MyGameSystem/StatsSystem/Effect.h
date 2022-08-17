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
	 * Gets the StatsComponent related to this effect.
	 * @return Related stats component.
	 */
	UFUNCTION(BlueprintPure, Category = "Effect|RelatedStatsComponent")
	class UStatsComponent* GetRelatedStatsComponent() const;

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
	FEffectConditionChangeDelegate OnEffectDeactivated;

protected:

	// Called after the destruction of this effect (deactivation).
	virtual void EndPlay_Implementation() override;

	/**
	 * Called after the activation.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control", meta = (BlueprintProtected))
	void OnActivated();
	virtual void OnActivated_Implementation() { return; }

	/**
	 * Called before the destruction.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Effect|Control", meta = (BlueprintProtected))
	void OnDeactivating();
	virtual void OnDeactivating_Implementation() { return; }

private:

	// Related stats component to this effect.
	class UStatsComponent* RelatedStatsComponent;

	// Notify all clients of the activation.
	UFUNCTION(NetMulticast, Reliable)
	void Notify_OnActivated();

	// Call OnActivated delegate.
	void Broadcast_OnActivated();

	// Call OnDeactivated delegate.
	void Broadcast_OnDeactivated();
	
};
