// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StorageInterface.h"
#include "ComplexStorageInterface.h"

#include "InventoryComponent.generated.h"

class AItem;
class UItemPlace;

/**
 * Delegate for handling dropping of items.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemDroppedDelegate, UInventoryComponent*, Inventory, AItem*, Item);

/**
 * Delegate for handling places.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryPlaceConditionDelegate, UInventoryComponent*, Inventory, UItemPlace*, ItemPlace);

/**
 * Inventory component for an actor.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class MYGAMESYSTEMPLUGIN_API UInventoryComponent : public UActorComponent, public IStorageInterface, public IComplexStorageInterface
{
	GENERATED_BODY()

public:	
	// Constructor.
	UInventoryComponent();

	// Override for replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Storage interface.
	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const;

	// Complex storage interface.
	virtual TArray<UItemPlace*> GetPlaces_Implementation() const { return ItemPlaces; }

	/**
	 * Connect an item place to this inventory.
	 * @param ItemPlace - The item place to connect.
	 * @return Successfully connected.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent|Control")
	bool AddPlace(UItemPlace* ItemPlace);

	/**
	 * Disconnect an item place from this inventory.
	 * @param ItemPlace - The item place to disconnect.
	 * @return Successfully removed.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent|Control")
	bool RemovePlace(UItemPlace* ItemPlace);

	/**
	 * Drop the item.
	 * @param Item - Item to drop.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryComponent|Control")
	void DropItem(AItem* Item);

	/**
	 * Called after an item has been dropped.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InventoryComponent|Delegates")
	FItemDroppedDelegate OnItemDropped;

	/**
	 * Called after an item place connection.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InventoryComponent|Delegates")
	FInventoryPlaceConditionDelegate OnPlaceAdded;

	/**
	 * Called after an item place disconnection.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InventoryComponent|Delegates")
	FInventoryPlaceConditionDelegate OnPlaceRemoved;

protected:

	/**
	 * Called after an item has been dropped.
	 * @param Item - The dropped item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected), Category = "InventoryComponent|Control")
	void ItemDropped(AItem* Item);
	virtual void ItemDropped_Implementation(AItem* Item) { return; }

private:

	UPROPERTY(ReplicatedUsing = OnRep_Places)
	TArray<UItemPlace*> ItemPlaces;

	UFUNCTION()
	void OnRep_Places(const TArray<UItemPlace*>& PrevItemPlaces);

	void BroadcastChange_Places(const TArray<UItemPlace*>& PrevItemPlaces);

	void Broadcast_OnItemPlaceAdded(UItemPlace* ItemPlace);
	void Broadcast_OnItemPlaceRemoved(UItemPlace* ItemPlace);

	UFUNCTION(NetMulticast, Reliable)
	void Notify_ItemDropped(AItem* Item);

	void Broadcast_ItemDropped(AItem* Item);
		
};
