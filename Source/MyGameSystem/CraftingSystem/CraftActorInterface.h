// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CraftActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCraftActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYGAMESYSTEM_API ICraftActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CraftActorInterface")
	class UInventoryComponent* GetCraftInventoryComponent();
	virtual class UInventoryComponent* GetCraftInventoryComponent_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "CraftActorInterface")
	class URecipeBookComponent* GetRecipeBookComponent();
	virtual class URecipeBookComponent* GetRecipeBookComponent_Implementation() = 0;
};
