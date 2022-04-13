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
 * 
 */
class MYGAMESYSTEM_API IMarkableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MarkableActorInterface")
	FTransform GetMarkerRelativeTransform() const;
	virtual FTransform GetMarkerRelativeTransfrom_Implementation() const { return FTransform(); }
};
