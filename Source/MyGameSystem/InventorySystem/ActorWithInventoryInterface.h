// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorWithInventoryInterface.generated.h"

class UInventoryComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorWithInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors with inventories.
 */
class MYGAMESYSTEM_API IActorWithInventoryInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get the inventory of this actor.
	 * @return Inventory.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorWithInventoryInterface")
	UInventoryComponent* GetInventoryComponent() const;
	virtual UInventoryComponent* GetInventoryComponent_Implementation() const = 0;

	/**
	 * Get the inventory manager of this actor.
	 * @return Inventory manager.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorWithInventoryInterface")
	UInventoryManagerComponent* GetInventoryManagerComponent() const;
	virtual UInventoryManagerComponent* GetInventoryManagerComponent_Implementation() const = 0;

	/**
	 * Get the drop transform for the inventory of this actor.
	 * @return Drop transform.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ActorWithInventoryInterface")
	FTransform GetDropTransform() const;
	virtual FTransform GetDropTransform_Implementation() const = 0;
};
