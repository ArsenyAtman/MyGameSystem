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
 * 
 */
class MYGAMESYSTEM_API IComplexStorageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ComplexStorageInterface")
	TArray<UItemPlace*> GetPlaces() const;
	virtual TArray<UItemPlace*> GetPlaces_Implementation() const = 0;
};
