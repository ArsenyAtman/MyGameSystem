// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatEffect.h"
#include "PassiveEffect.generated.h"

/**
 * 
 */
UCLASS()
class MYGAMESYSTEM_API UPassiveEffect : public UStatEffect
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "PassiveEffect|ApplyToDelta")
	FStatValues ApplyToDelta(FStatValues Delta, class UEffect* OfEffect, class UStat* OnStat);
	virtual FStatValues ApplyToDelta_Implementation(FStatValues Delta, class UEffect* OfEffect, class UStat* OnStat) { return FStatValues(); }
	
};
