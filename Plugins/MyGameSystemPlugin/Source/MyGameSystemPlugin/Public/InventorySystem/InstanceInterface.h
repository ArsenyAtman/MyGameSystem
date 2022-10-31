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
 * Interface for instancing.
 */
class MYGAMESYSTEMPLUGIN_API IInstanceInterface
{
	GENERATED_BODY()

public:

	/**
	 * Instance or uninstance the object.
	 * @param bNewIsInstanced - Is instanced or not.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InstancedInterface")
	void SetIsInstanced(bool bNewIsInstanced);
	virtual void SetIsInstanced_Implementation(bool bNewIsInstanced) = 0;
};
