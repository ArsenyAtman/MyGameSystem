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
 * 
 */
class MYGAMESYSTEM_API IActionDrivenActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActionDrivenActorInterface")
	class UActionDriverComponent* GetActionDriverComponent();
	virtual class UActionDriverComponent* GetActionDriverComponent_Implementation() = 0;
};
