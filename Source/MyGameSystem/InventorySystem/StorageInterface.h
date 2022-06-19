// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StorageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStorageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IStorageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<UItemPlace*> GetItemPlaces() const;
	virtual TArray<UItemPlace*> GetItemPlaces_Implementation() const = 0;

};
