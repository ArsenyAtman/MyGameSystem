// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InstancedInterface")
	void SetIsInstanced(bool bNewIsInstanced);
	virtual void SetIsInstanced_Implementation(bool bNewIsInstanced) = 0;
};
