// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "Stat.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStatPassiveEffectDelegate, class UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStatValueChangeDelegate, FStatValues, Delta, class UEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStatConditionChangeDelegate);

USTRUCT(BlueprintType, Blueprintable)
struct FStatInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatDataAsset* StatData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatValues StatBaseValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatValues StatValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FEffectInfo> AppliedEffects;

	FStatInfo(class UStatDataAsset* Data = nullptr, FStatValues BaseValues = FStatValues(), FStatValues Values = FStatValues(), TArray<struct FEffectInfo> Effects = TArray<struct FEffectInfo>())
	{
		StatData = Data;
		StatBaseValues = BaseValues;
		StatValues = Values;
		AppliedEffects = Effects;
	}
};

UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class MYGAMESYSTEM_API UStat : public UObject
{
	GENERATED_BODY()

public:

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

	UFUNCTION(BlueprintPure, Category = "Stat|Condition")
	FStatInfo GetStatInfo() const;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatValueChangeDelegate OnValuesChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatConditionChangeDelegate OnMinReached;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatConditionChangeDelegate OnMaxReached;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatPassiveEffectDelegate OnEffectAdded;

	UPROPERTY(BlueprintAssignable, Category = "Stat|Delegates")
	FStatPassiveEffectDelegate OnEffectRemoved;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatValues, Category = "Stat|Condition")
	FStatValues StatValues;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetStatBaseValues, Category = "Stat|Condition")
	FStatValues StatBaseValues;

	UPROPERTY(BlueprintGetter = GetEffects, Category = "Stat|Effects")
	TArray<class UEffect*> Effects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat|Condition")
	class UStatDataAsset* StatData = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Stat|DeltaApplier")
	class UStatDeltaApplier* StatDeltaApplier;
};
