// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StorageInterface.generated.h"

class AItem;
class UItemPlace;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStorageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for a storage with items.
 */
class MYGAMESYSTEMPLUGIN_API IStorageInterface
{
	GENERATED_BODY()

public:

	/**
	 * Add an item to the storage.
	 * @param Item - An item to add.
	 * @return Whether the item has been added.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StorageInterface")
	bool AddItem(AItem* Item);
	virtual bool AddItem_Implementation(AItem* Item) = 0;

	/**
	 * Find all items of a class.
	 * @param ItemClass - A class of items.
	 * @return List of found items.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StorageInterface")
	TArray<AItem*> FindItemsByClass(TSubclassOf<AItem> ItemClass) const;
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const = 0;

	/**
	 * Check if an item belongs to this storage or its subitems.
	 * @param Item - An item to check.
	 * @return Belongs or not.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StorageInterface")
	bool CheckItemPossession(AItem* Item) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const = 0;

};
