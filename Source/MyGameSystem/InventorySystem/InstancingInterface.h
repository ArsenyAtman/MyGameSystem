// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InstancingInterface.generated.h"


class AItemInstance;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInstancingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IInstancingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Instance(AItemInstance* ParentInstance);
	virtual void Instance_Implementation(AItemInstance* ParentInstance) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Uninstance();
	virtual void Uninstance_Implementation() = 0;

};
