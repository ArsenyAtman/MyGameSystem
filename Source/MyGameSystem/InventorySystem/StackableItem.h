// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StorageInterface.h"

#include "StackableItem.generated.h"

class AItem;

/**
 * Delegate for handling changes of the count of items in a stack.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCountInStackChangeDelegate, AStackableItem*, StackableItem, int32, NewCountInStack, int32, PrevCountInStack);

/**
 * An item that can stacks.
 */
UCLASS()
class MYGAMESYSTEM_API AStackableItem : public AItem, public IStorageInterface
{
	GENERATED_BODY()

public:

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Storage interface
	virtual bool AddItem_Implementation(AItem* Item) override;
	virtual TArray<AItem*> FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const override { return TArray<AItem*>(); }
	virtual bool CheckItemPossession_Implementation(AItem* Item) const;

	/**
	 * Set the initial count in stack.
	 * @param InitCountInStack - The initial count.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "StackableItem|Internal")
	void InitializeCountInStack(int32 InitCountInStack);

	/**
	 * Get the count of items in this stack.
	 * @return Count of items.
	 */
	UFUNCTION(BlueprintGetter, Category = "StackableItem|Count")
	int32 GetCountInStack() const { return CountInStack; }

	/**
	 * Get the maximal count of items in this stack.
	 * @return Max count of items.
	 */
	UFUNCTION(BlueprintGetter, Category = "StackableItem|Count")
	int32 GetMaxCountInStack() const { return MaxCountInStack; }

	/**
	 * This item has been merged with another stackable.
	 * @param CountOfTakenItems - Count of taken items from this stackable item.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "StackableItem|Internal")
	void MergedWithItem(int32 CountOfTakenItems);

	/**
	 * Split this item.
	 * @param CountToTake - Count of items to take from this stack.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "StackableItem|Control")
	AStackableItem* Split(int32 CountToTake);

	/**
	 * Called after the count of items has changed.
	 */
	UPROPERTY(BlueprintAssignable, Category = "StackableItem|Delegates")
	FCountInStackChangeDelegate OnCountInStackChanged;

protected:

	/**
	 * Merge this stack with another.
	 * @param Item - An item to merge with.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "StackableItem|Internal")
	int32 MergeWithItem(AStackableItem* Item);

	/**
	 * Set count of items in this stack.
	 * @param NewCountInStack - Count of taken items from this stackable item.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintSetter, meta = (BlueprintProtected), Category = "StackableItem|Internal")
	void SetCountInStack(int32 NewCountInStack);

	/**
	 * Called after the count of items has changed.
	 * @param CurrentCountInStack - Current count of items.
	 * @param PrevCountInStack - Previous count of items.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected), Category = "StackableItem|Control")
	void CountInStackChanged(int32 CurrentCountInStack, int32 PrevCountInStack);
	virtual void CountInStackChanged_Implementation(int32 CurrentCountInStack, int32 PrevCountInStack) { return; }

private:

	/**
	 * Current count of items.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetCountInStack, BlueprintSetter = SetCountInStack, ReplicatedUsing = OnRep_CountInStack, meta = (AllowPrivateAccess), Category = "StackableItem|Count")
	int32 CountInStack;

	/**
	 * Max count of items.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxCountInStack, meta = (AllowPrivateAccess), Category = "StackableItem|Count")
	int32 MaxCountInStack;

	UFUNCTION()
	void OnRep_CountInStack(int32 PrevCountInStack);

	void Broadcast_CountInStackChanged(int32 PrevCountInStack);
	
};
