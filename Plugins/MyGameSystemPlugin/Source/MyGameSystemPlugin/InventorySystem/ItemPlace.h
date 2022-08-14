// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "StorageInterface.h"
#include "InstanceInterface.h"
#include "SizedInterface.h"

#include "ItemPlace.generated.h"

class AItem;
class UObject;
class UItemPlace;
class UItemLocator;

/**
 * Delegate for handling condition changes of a place.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemPlaceConditionDelegate, UItemPlace*, ItemPlace, AItem*, Item);

/**
 * Storage for items.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UItemPlace : public USceneComponent, public IStorageInterface, public IInstanceInterface, public ISizedInterface
{
	GENERATED_BODY()

public:

	// Constructor.
	UItemPlace();

	// Replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Get items of this place.
	 * @return The items.
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "ItemPlace|Items")
	TArray<AItem*> GetItems() const;
	virtual TArray<AItem*> GetItems_Implementation() const { return Items; }

	// Storage interface.
	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const;

	// Instance interface.
	virtual void SetIsInstanced_Implementation(bool bNewIsInstanced) override;

	// SizedInterface
	virtual FVector2D GetInventorySize_Implementation() const override { return PlaceSize; }

	/**
	 * Get the inventory component that stores this item.
	 * @return The related inventory.
	 */
	UFUNCTION(BlueprintPure, Category = "ItemPlace|Inventory")
	UInventoryComponent* GetRelatedInventory() const;

	/**
	 * Get the posessor of this item.
	 * @return The possessor of this item.
	 */
	UFUNCTION(BlueprintGetter, Category = "ItemPlace|Inventory")
	UObject* GetPossessor() const;

	/**
	 * Place a new item in this place.
	 * @param NewItem - A new item to add.
	 * @param NewItemPosition - A new inventory location for the item.
	 * @return Is successfully placed.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemPlace|Control")
	bool PlaceItem(AItem* NewItem, FVector2D NewItemPosition);

	/**
	 * Remove an item from this place.
	 * @param Item - An item to remove.
	 * @return Is successfully removed.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemPlace|Control")
	bool RemoveItem(AItem* Item);

	/**
	 * Get is this place instancing its subitems.
	 * @return Is instancing.
	 */
	UFUNCTION(BlueprintGetter, Category = "ItemPlace|Items")
	bool GetIsInstancing() { return bIsInstancing; }

	/**
	 * Set is this place instancing its subitems.
	 * @param bNewIsInstancing - Is instancing.
	 */
	UFUNCTION(BlueprintSetter, Category = "ItemPlace|Items")
	void SetIsInstancing(bool bNewIsInstancing);

	/**
	 * Get relative transform for subitems.
	 * @param Item - For item.
	 * @return Relative transform for the item.
	 */
	UFUNCTION(BlueprintPure, Category = "ItemPlace|Items")
	FTransform GetRelativeTransformForItem(AItem* Item);

	/**
	 * Called after a new item has been placed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ItemPlace|Delegates")
	FItemPlaceConditionDelegate OnPlaced;

	/**
	 * Called after a new item has been removed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ItemPlace|Delegates")
	FItemPlaceConditionDelegate OnRemoved;

protected:

	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected), Category = "ItemPlace|Items")
	UItemLocator* ItemLocator;

	/**
	 * Called after a new item has been placed.
	 * @param Item - A placed item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected), Category = "ItemPlace|Control")
	void ItemPlaced(AItem* Item);
	virtual void ItemPlaced_Implementation(AItem* Item) { return; }

	/**
	 * Called after a new item has been removed.
	 * @param Item - A removed item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected), Category = "ItemPlace|Control")
	void ItemRemoved(AItem* Item);
	virtual void ItemRemoved_Implementation(AItem* Item) { return; }

private:

	/**
	 * Size of this place
	 */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true), Category = "ItemPlace|Inventory")
	FIntPoint PlaceSize;

	/**
	 * Array of subitems.
	 */
	UPROPERTY(ReplicatedUsing = OnRep_Items)
	TArray<AItem*> Items;

	/**
	 * Is this place instancing its subitems.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetIsInstancing, BlueprintSetter = SetIsInstancing, meta = (AllowPrivateAccess = true), Category = "ItemPlace|Items")
	bool bIsInstancing = true;

	UFUNCTION()
	void OnRep_Items(const TArray<AItem*>& PrevItems);

	void BroadcastChange_Items(const TArray<AItem*>& PrevItems);
	void Broadcast_ItemPlaced(AItem* Item);
	void Broadcast_ItemRemoved(AItem* Item);
	
};
