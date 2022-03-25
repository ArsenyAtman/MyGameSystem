// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatsSystemTypes.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FStatValues
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max;

	FStatValues(float StatCurrentValue = 0.0f, float StatMinValue = 0.0f, float StatMaxValue = 0.0f)
	{
		Value = StatCurrentValue;
		Min = StatMinValue;
		Max = StatMaxValue;
	}

	FStatValues operator + (FStatValues StatValues);
	FStatValues operator - (FStatValues StatValues);
	FStatValues operator * (FStatValues StatValues);
	FStatValues operator / (FStatValues StatValues);

	FStatValues operator * (float Scale);
	friend FStatValues operator * (float Scale, FStatValues StatValues);

	FStatValues& operator += (FStatValues StatValues);
	FStatValues& operator -= (FStatValues StatValues);
	FStatValues& operator *= (FStatValues StatValues);
	FStatValues& operator /= (FStatValues StatValues);

	FStatValues& operator *= (float Scale);
};

USTRUCT(BlueprintType, Blueprintable)
struct FEffectInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEffect> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EffectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EffectDescription;

	FEffectInfo()
	{

	}

	FEffectInfo(TSubclassOf<class UEffect> Class, FText Name, FText Description)
	{
		EffectClass = Class;
		EffectName = Name;
		EffectDescription = Description;
	}
};

USTRUCT(BlueprintType, Blueprintable)
struct FStatInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UStat> StatClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatValues StatValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectInfo> AppliedEffects;

	FStatInfo()
	{

	}

	FStatInfo(TSubclassOf<class UStat> Class, FText Name, FText Description, FStatValues Values, TArray<FEffectInfo> Effects)
	{
		StatClass = Class;
		StatName = Name;
		StatDescription = Description;
		StatValues = Values;
		AppliedEffects = Effects;
	}
};
