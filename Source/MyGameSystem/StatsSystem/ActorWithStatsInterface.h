// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorWithStatsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorWithStatsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IActorWithStatsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorWithStatsInterface")
	class UStatsComponent* GetStatsComponent();
	virtual class UStatsComponent* GetStatsComponent_Implementation() = 0;
};
