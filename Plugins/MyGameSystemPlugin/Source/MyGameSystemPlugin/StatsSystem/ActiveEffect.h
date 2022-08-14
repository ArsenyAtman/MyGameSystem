// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatEffect.h"
#include "ActiveEffect.generated.h"

/**
 * Effect that applies it's delta to related stats.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UActiveEffect : public UStatEffect
{
	GENERATED_BODY()

protected:

	/**
	 * Apply the delta of this effect.
	 * @param Delta - The value to apply.
	 * @return Delta that has been applied to all stats.
	 */
	UFUNCTION(BlueprintCallable, Category = "ActiveEffect|ApplyDelta", meta = (BlueprintProtected))
	FStatValues ApplyDeltaToRelatedStats(FStatValues Delta);
	
};
