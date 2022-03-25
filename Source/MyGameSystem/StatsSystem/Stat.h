// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "Stat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatPassiveEffectDelegate, class UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatValueChangeDelegate, FStatValues, Delta, class UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatConditionChangeDelegate);

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MYGAMESYSTEM_API UStat : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	FStatValues ApplyDelta(FStatValues Delta, class UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void AddEffect(class UEffect* Effect);

	UFUNCTION(BlueprintCallable)
	void RemoveEffect(class UEffect* Effect);

	UFUNCTION(BlueprintGetter)
	FStatValues GetStatValues() { return StatValues; }

	UFUNCTION(BlueprintGetter)
	FStatValues GetStatBaseValues() { return StatBaseValues; }

	UFUNCTION(BlueprintGetter)
	TArray<class UEffect*> GetEffects() { return Effects; }

	UFUNCTION(BlueprintPure)
	class UStatsComponent* GetOwningStatsComponent();

	UFUNCTION(BlueprintPure)
	FStatInfo GetStatInfo();

	UPROPERTY(BlueprintAssignable)
	FStatValueChangeDelegate OnValuesChanged;

	UPROPERTY(BlueprintAssignable)
	FStatConditionChangeDelegate OnMinReached;

	UPROPERTY(BlueprintAssignable)
	FStatConditionChangeDelegate OnMaxReached;

	UPROPERTY(BlueprintAssignable)
	FStatPassiveEffectDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable)
	FStatPassiveEffectDelegate OnEffectRemoved;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText StatDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatValues)
	FStatValues StatValues;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatBaseValues)
	FStatValues StatBaseValues;

	UPROPERTY(BlueprintGetter = GetEffects)
	TArray<class UEffect*> Effects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced)
	class UStatDeltaApplier* StatDeltaApplier;
};
