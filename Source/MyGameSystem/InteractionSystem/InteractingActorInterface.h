// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractingActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractingActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IInteractingActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InteractingActorInterface")
	class USceneComponent* GetTraceSceneComponent() const;
	virtual class USceneComponent* GetTraceSceneComponent_Implementation() const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InteractingActorInterface")
	class UInteractionComponent* GetInteractionComponent() const;
	virtual class UInteractionComponent* GetInteractionComponent_Implementation() const = 0;
};
