// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ComplexStorageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UComplexStorageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for storages with subplaces.
 */
class MYGAMESYSTEMPLUGIN_API IComplexStorageInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get places of this storage.
	 * @return Places of this storage.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ComplexStorageInterface")
	TArray<UItemPlace*> GetPlaces() const;
	virtual TArray<UItemPlace*> GetPlaces_Implementation() const = 0;
};
