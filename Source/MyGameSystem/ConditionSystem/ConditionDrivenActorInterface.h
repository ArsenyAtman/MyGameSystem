// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ConditionDrivenActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConditionDrivenActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors with a condition component.
 */
class MYGAMESYSTEM_API IConditionDrivenActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * Get a ConditionComponent of this actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ConditionDrivenActorInterface")
	class UConditionComponent* GetConditionComponent() const;
	virtual class UConditionComponent* GetConditionComponent_Implementation() const = 0;
};
