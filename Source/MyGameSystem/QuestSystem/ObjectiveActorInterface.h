// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectiveActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UObjectiveActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IObjectiveActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ObjectiveActorInterface")
	TArray<class AActor*> GetObjectives();
	virtual TArray<class AActor*> GetObjectives_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ObjectiveActorInterface")
	FTransform GetMarkerRelativeTransform();
	virtual FTransform GetMarkerRelativeTransfrom_Implementation() { return FTransform(); }
};
