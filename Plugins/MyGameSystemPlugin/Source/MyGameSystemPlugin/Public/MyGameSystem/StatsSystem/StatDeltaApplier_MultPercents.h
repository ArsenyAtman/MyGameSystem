// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatDeltaApplier.h"
#include "StatDeltaApplier_MultPercents.generated.h"

/**
 * DeltaApplier that multiplies all delta modifications of passive effects.
 */
UCLASS()
class MYGAMESYSTEMPLUGIN_API UStatDeltaApplier_MultPercents : public UStatDeltaApplier
{
	GENERATED_BODY()

public:

	virtual FStatValues ApplyDelta_Implementation(FStatValues Delta, class UEffect* OfEffect) override;
	
};
