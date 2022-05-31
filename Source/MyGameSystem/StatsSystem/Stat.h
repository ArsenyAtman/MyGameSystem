// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameSystem/AdvancedObject/AdvancedObject.h"
#include "StatsSystemTypes.h"
#include "Stat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatEffectDelegate, class UStat*, Stat, class UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatValueChangeDelegate, class UStat*, Stat, FStatValues, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatConditionChangeDelegate, class UStat*, Stat);

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MYGAMESYSTEM_API UStat : public UAdvancedObject
{
	GENERATED_BODY()

public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Stat|Delta")
	FStatValues ApplyDelta(FStatValues Delta, class UEffect* Effect);

	UFUNCTION(BlueprintCallable, Category = "Stat|Effects")
	void AddEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable, Category = "Stat|Effects")
	void RemoveEffect(class UEffect* Effect);

	UFUNCTION(BlueprintGetter, Category = "Stat|Condition")
	FStatValues GetStatValues() const { return StatValues; }

	UFUNCTION(BlueprintGetter, Category = "Stat|Condition")
	FStatValues GetStatBaseValues() const { return StatBaseValues; }

	UFUNCTION(BlueprintGetter, Category = "Stat|Effects")
	TArray<class UEffect*> GetEffects() const { return Effects; }

	UFUNCTION(BlueprintPure, Category = "Stat|StatsComponent")
	class UStatsComponent* GetOwningStatsComponent() const;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatValueChangeDelegate OnValuesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatConditionChangeDelegate OnMinReached;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatConditionChangeDelegate OnMaxReached;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatEffectDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatEffectDelegate OnEffectRemoved;

protected:

	UFUNCTION(BlueprintSetter, meta = (BlueprintProtected))
	void SetStatValues(FStatValues NewStatValues);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatValues,  BlueprintSetter = SetStatValues, ReplicatedUsing = OnRep_StatValues, Category = "Stat|Condition", meta = (AllowPrivateAccess = true))
	FStatValues StatValues;

	UFUNCTION()
	void OnRep_StatValues(FStatValues PreReplicationStatValues);

	UFUNCTION()
	void BroadcastChange_StatValues(FStatValues PrevValues);

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatBaseValues, Category = "Stat|Condition", meta = (AllowPrivateAccess = true))
	FStatValues StatBaseValues;

	UPROPERTY(BlueprintGetter = GetEffects, ReplicatedUsing = OnRep_Effects, Category = "Stat|Effects")
	TArray<class UEffect*> Effects;

	UFUNCTION()
	void OnRep_Effects(const TArray<class UEffect*>& PreReplicationEffects);

	UFUNCTION()
	void BroadcastChange_Effects(const TArray<class UEffect*>& PrevEffects);

	UFUNCTION()
	void Broadcast_OnEffectAdded(class UEffect* Effect);

	UFUNCTION()
	void Broadcast_OnEffectRemoved(class UEffect* Effect);

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Stat|DeltaApplier")
	class UStatDeltaApplier* StatDeltaApplier;

	TArray<class UEffect*> FindMissingEffects(const TArray<class UEffect*>& FromArray, const TArray<class UEffect*>& InArray) const;
};
