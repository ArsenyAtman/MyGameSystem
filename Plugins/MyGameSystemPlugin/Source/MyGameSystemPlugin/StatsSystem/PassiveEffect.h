// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatEffect.h"
#include "PassiveEffect.generated.h"

/**
 * Effect that modifies the incoming delta of stats.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UPassiveEffect : public UStatEffect
{
	GENERATED_BODY()

public:

	/**
	 * Modify the incoming delta.
	 * @param Delta - The values to modify.
	 * @param OfEffect - Of the effect.
	 * @param OnStat - Applied to the stat.
	 * @return Modified delta.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PassiveEffect|ApplyToDelta")
	FStatValues ApplyToDelta(FStatValues Delta, class UEffect* OfEffect, class UStat* OnStat);
	virtual FStatValues ApplyToDelta_Implementation(FStatValues Delta, class UEffect* OfEffect, class UStat* OnStat) { return FStatValues(); }
	
};
