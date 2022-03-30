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
