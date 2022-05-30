// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActionDrivenActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActionDrivenActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for an actor with ActionDriverComponent.
 */
class MYGAMESYSTEM_API IActionDrivenActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get ActionDriver of this actor.
	 * @return ActionDriverComponent that controls this actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionDrivenActorInterface")
	class UActionDriverComponent* GetActionDriverComponent() const;
	virtual class UActionDriverComponent* GetActionDriverComponent_Implementation() const = 0;
};
