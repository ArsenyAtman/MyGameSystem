// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "StatDeltaApplier.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class MYGAMESYSTEM_API UStatDeltaApplier : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FStatValues ApplyDelta(FStatValues Delta, class UEffect* OfEffect);
	virtual FStatValues ApplyDelta_Implementation(FStatValues Delta, class UEffect* OfEffect) { return FStatValues(); }

	UFUNCTION(BlueprintPure)
	class UStat* GetOwningStat();
	
};
