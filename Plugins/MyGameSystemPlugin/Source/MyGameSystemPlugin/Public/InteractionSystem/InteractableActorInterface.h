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
 * Interface for interactive objects.
 */
class MYGAMESYSTEMPLUGIN_API IInteractableActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Interact with this interactable.
	 * @param Actor - An actor that interacting.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionInterface")
	void Interact(class AActor* Actor);
	virtual void Interact_Implementation(class AActor* Actor) = 0;

	/**
	 * Whether or not the specified actor can interact with this interactable.
	 * @param Actor - An actor that possibly can interact.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionInterface")
	bool CanInteract(class AActor* Actor) const;
	virtual bool CanInteract_Implementation(class AActor* Actor) const { return true; }

	/**
	 * Get a description of the possible interaction with this interactable.
	 * @return Text of the description.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionInterface")
	FText GetActionDescription() const;
	virtual FText GetActionDescription_Implementation() const { return FText::FromString("None"); }
};
