// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorWithInventoryInterface.generated.h"


class UInventoryComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorWithInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IActorWithInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInventoryComponent* GetInventoryComponent();
	virtual UInventoryComponent* GetInventoryComponent_Implementation() = 0;
};
