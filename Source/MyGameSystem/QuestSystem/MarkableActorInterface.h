// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MarkableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMarkableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * An interface for actors that can be marked as an objective.
 */
class MYGAMESYSTEM_API IMarkableActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Returns a relative transform for markers.
	 * @return The relative transform for markers.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MarkableActorInterface")
	FTransform GetMarkerRelativeTransform() const;
	virtual FTransform GetMarkerRelativeTransfrom_Implementation() const { return FTransform(); }
};
