// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatsSystemTypes.h"
#include "StatDeltaApplier.generated.h"

/**
 * Object that controls modification the incoming delta.
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class MYGAMESYSTEM_API UStatDeltaApplier : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Modify delta values.
	 * @param Delta - The delta values.
	 * @param OfEffect - Of the effect.
	 * @return Modified delta.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StatDeltaApplier|ApplyDelta")
	FStatValues ApplyDelta(FStatValues Delta, class UEffect* OfEffect);
	virtual FStatValues ApplyDelta_Implementation(FStatValues Delta, class UEffect* OfEffect) { return FStatValues(); }

	/**
	 * Gets the owning stat.
	 * @return The stat that owns this applier.
	 */
	UFUNCTION(BlueprintPure, Category = "StatDeltaApplier|OwningStat")
	class UStat* GetOwningStat() const;
	
};
