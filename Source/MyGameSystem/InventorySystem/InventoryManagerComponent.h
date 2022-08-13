// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"

#include "InventoryManagerComponent.generated.h"


class AItem;
class AComplexItem;
class AStackableItem;
class UInventoryComponent;

/**
 * Delegate for connections handling.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryManagerConnectionDelegate, UInventoryManagerComponent*, InventoryManager, UInventoryComponent*, InventoryComponent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYGAMESYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Constructor.
	UInventoryManagerComponent();

	// Replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * Move the item into a new posession.
	 * @param Item - Item to move.
	 * @param NewPossessionInfo - New possession info.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void ChangeItemPossession(AItem* Item, FItemPossessionInfo NewPossessionInfo);

	/**
	 * Pickup an unpossessed item.
	 * @param Item - Item to pickup.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void PickupItem(AItem* Item);

	/**
	 * Drop an item.
	 * @param Item - Item to drop.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void DropItem(AItem* Item);

	/**
	 * Place an item in a complex item.
	 * @param Item - Item to place.
	 * @param ComplexItem - Complex item to store.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void AddItemToComplexItem(AItem* Item, AComplexItem* ComplexItem);

	/**
	 * Merge two stackable items.
	 * @param ReceivingItem - Item that is receiving incoming stack.
	 * @param IncomingItem - An incoming stack.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void MergeStackableItems(AStackableItem* ReceivingItem, AStackableItem* IncomingItem);

	/**
	 * Split a stackable item.
	 * @param ItemToSplit - A stackable item to split.
	 * @param CountToTake - Count of items to take from.
	 * @param PosessionInfo - Possession info for a new item.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void SplitStackableItem(AStackableItem* ItemToSplit, int32 CountToTake, FItemPossessionInfo PossessionInfo);

	/**
	 * Move amount of items form one stack to another.
	 * @param ReceivingItem - Item that is receiving incoming stack.
	 * @param ItemToSplit - Item that is giving incoming stack.
	 * @param CountToTake - Count of items to move.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "InventoryManager|Control")
	void SplitAndMergeStackableItem(AStackableItem* ReceivingItem, AStackableItem* ItemToSplit, int32 CountToTake);

	/**
	 * Connect a new inventory to this manager.
	 * @param InventoryComponent - Inventory to connect.
	 * @return Is successfully connected.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryManager|Connections")
	bool Connect(UInventoryComponent* InventoryComponent);

	/**
	 * Disconnect a connected inventory from this manager.
	 * @param InventoryComponent - Inventory to disconnect.
	 * @return Is successfully disconnected.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "InventoryManager|Connections")
	bool Disconnect(UInventoryComponent* InventoryComponent);

	/**
	 * Get the all connections of this manager.
	 * @return Connections.
	 */
	UFUNCTION(BlueprintGetter, Category = "InventoryManager|Connections")
	TArray<UInventoryComponent*> GetConnections() const { return Connections; }

	/**
	 * Get the inventory component of the owner.
	 * @return The inventory component of the owner.
	 */
	UFUNCTION(BlueprintPure, Category = "InventoryManager|Control")
	UInventoryComponent* GetInventoryOfOwner() const;

	/**
	 * Called after a new inventory has been connected.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InventoryManager|Delegates")
	FInventoryManagerConnectionDelegate OnInventoryConnected;

	/**
	 * Called after an inventory has been disconnected.
	 */
	UPROPERTY(BlueprintAssignable, Category = "InventoryManager|Delegates")
	FInventoryManagerConnectionDelegate OnInventoryDisconnected;

protected:

	// ...

private:

	/**
	 * Connected inventories to this manager (inventories that player can manage).
	 */
	UPROPERTY(BlueprintGetter = GetConnections, ReplicatedUsing = OnRep_Connections, meta = (AllowPrivateAccess), Category = "InventoryManager|Connections")
	TArray<UInventoryComponent*> Connections;

	UFUNCTION()
	void OnRep_Connections(const TArray<UInventoryComponent*>& PrevConnections);
		
};
