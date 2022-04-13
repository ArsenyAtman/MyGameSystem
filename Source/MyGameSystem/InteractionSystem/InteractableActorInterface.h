// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API IInteractableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionInterface")
	void Interact(class AActor* Actor) const;
	virtual void Interact_Implementation(class AActor* Actor) const = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionInterface")
	bool CanInteract(class AActor* Actor) const;
	virtual bool CanInteract_Implementation(class AActor* Actor) const { return true; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionInterface")
	FText GetActionDescription() const;
	virtual FText GetActionDescription_Implementation() const { return FText::FromString("None"); }
};
