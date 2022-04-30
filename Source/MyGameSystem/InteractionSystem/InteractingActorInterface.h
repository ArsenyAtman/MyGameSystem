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
 * Interface for actors that can interact.
 */
class MYGAMESYSTEM_API IInteractingActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get a component that defines a line trace direction (to find an interactive object).
	 * @return A SceneComponent for the line tracing process.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InteractingActorInterface")
	class USceneComponent* GetTraceSceneComponent() const;
	virtual class USceneComponent* GetTraceSceneComponent_Implementation() const = 0;

	/**
	 * Get an interaction component of this actor.
	 * @return An InteractionComponent of this actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "InteractingActorInterface")
	class UInteractionComponent* GetInteractionComponent() const;
	virtual class UInteractionComponent* GetInteractionComponent_Implementation() const = 0;
};
