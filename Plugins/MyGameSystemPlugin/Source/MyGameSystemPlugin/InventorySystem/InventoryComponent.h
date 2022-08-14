// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StorageInterface.h"
#include "ComplexStorageInterface.h"

#include "InventoryComponent.generated.h"

class AItem;

/**
 * Delegate for handling dropping of items.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemDroppedDelegate, UInventoryComponent*, Inventory, AItem*, Item);

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

	// Storage interface.
	virtual bool AddItem_Implementation(AItem* Item);
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const;
	virtual bool CheckItemPossession_Implementation(AItem* Item) const;

	// Complex storage interface.
	virtual TArray<UItemPlace*> GetPlaces_Implementation() const;

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

protected:

	/**
	 * Called after an item has been dropped.
	 * @param Item - The dropped item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected), Category = "InventoryComponent|Control")
	void ItemDropped(AItem* Item);
	virtual void ItemDropped_Implementation(AItem* Item) { return; }

private:

	UFUNCTION(NetMulticast, Reliable)
	void Notify_ItemDropped(AItem* Item);

	void Broadcast_ItemDropped(AItem* Item);
		
};
