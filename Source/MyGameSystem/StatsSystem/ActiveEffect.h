// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatEffect.h"
#include "ActiveEffect.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UActiveEffect : public UStatEffect
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category = "ActiveEffect|ApplyDelta")
	FStatValues ApplyDeltaToRelatedStats(FStatValues Delta);
	
};
