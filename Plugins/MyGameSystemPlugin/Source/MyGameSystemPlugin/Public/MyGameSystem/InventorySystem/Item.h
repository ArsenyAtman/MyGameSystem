// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstanceInterface.h"
#include "SizedInterface.h"

#include "Item.generated.h"

class AItem;
class UItemPlace;
class UInventoryComponent;
class UItemResizer;

/**
 * Information about an item posession.
 */
USTRUCT(BlueprintType, Blueprintable)
struct MYGAMESYSTEMPLUGIN_API FItemPossessionInfo
{
	GENERATED_BODY()

public:

	/**
	 * A place that owns the item.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItemPlace* PossessingPlace;

	/**
	 * Location of the item.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D InventoryLocation;

	FItemPossessionInfo(UItemPlace* Place = nullptr, FVector2D Location = FVector2D::ZeroVector)
	{
		PossessingPlace = Place;
		InventoryLocation = Location;
	}
};

/**
 * Delegate for handling posession changes.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FItemPossessionChangeDelegate, AItem*, Item, FItemPossessionInfo, NewPossessionInfo, FItemPossessionInfo, PrevPossessionInfo);

/**
 * Delegate for handling condition changes.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemConditionChangeDelegate, AItem*, Item);

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API AItem : public AActor, public IInstanceInterface, public ISizedInterface
{
	GENERATED_BODY()

public:

	// Constructor.
	AItem();

	// Replication.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Instance interface.
	virtual void SetIsInstanced_Implementation(bool bNewIsInstanced) override;

	// Sized interface.
	virtual FVector2D GetInventoryLocation_Implementation() const override { return ItemPossession.InventoryLocation; }
	virtual FVector2D GetInventorySize_Implementation() const override;
	virtual FVector2D GetInventorySizeForPlace_Implementation(UItemPlace* Place) const override;

	/**
	 * Get the related inventory to this item.
	 * @return The related inventory component.
	 */
	UFUNCTION(BlueprintPure, Category = "Item|Condition")
	UInventoryComponent* GetRelatedInventory() const;

	/**
	 * Whether or not this item is currently instanced.
	 * @return Is instanced.
	 */
	UFUNCTION(BlueprintGetter, Category = "Item|Condition")
	bool GetIsInstanced() const { return bIsInstanced; }

	/**
	 * Get the possession info of this item.
	 * @return Possession info.
	 */
	UFUNCTION(BlueprintGetter, Category = "Item|Possession")
	FItemPossessionInfo GetItemPosession() const { return ItemPossession; }

	/**
	 * Get the possessing place of this item.
	 * @return Possessing place.
	 */
	UFUNCTION(BlueprintPure, Category = "Item|Possession")
	UItemPlace* GetPossessingPlace() const { return ItemPossession.PossessingPlace; }

	/**
	 * Remove this item from its owning place.
	 * @warning Server-only!
	 */
	UFUNCTION(BlueprintCallable, Category = "Item|Control")
	void RemoveFromCurrentPlace();

	/**
	 * Change the possession of this item.
	 * @warning Use this function only if you know what you are doing!
	 */
	UFUNCTION(BlueprintCallable, Category = "Item|Internal")
	void ChangePossession(FItemPossessionInfo NewPossessionInfo);

	/**
	 * Called after the posession changes.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Item|Delegates")
	FItemPossessionChangeDelegate OnPossessionChanged;

	/**
	 * Called after the IsInstanced changes.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Item|Delegates")
	FItemConditionChangeDelegate OnInstancedChanged;

protected:

	/**
	 * Item size controller.
	 */
	UPROPERTY(Instanced, EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintProtected), Category = "Item|Size")
	UItemResizer* ItemResizer;

	/**
	 * Called after the posession changes.
	 * @param NewPossessionInfo - New possession info.
	 * @param PrevPosessionInfo - Previous posession info.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected), Category = "Item|Control")
	void PossessionChanged(FItemPossessionInfo NewPosessionInfo, FItemPossessionInfo PrevPossessionInfo);
	virtual void PossessionChanged_Implementation(FItemPossessionInfo NewPosessionInfo, FItemPossessionInfo PrevPossessionInfo) { return; }

	/**
	 * Called after the IsInstanced changes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, meta = (BlueprintProtected),Category = "Item|Control")
	void IsInstancedChanged();
	virtual void IsInstancedChanged_Implementation() { return; }

private:

	/**
	 * Information about the item posession.
	 */
	UPROPERTY(BlueprintGetter = GetItemPosession, ReplicatedUsing = OnRep_ItemPossession, meta = (AllowPrivateAccess), Category = "Item|Posession")
	FItemPossessionInfo ItemPossession;

	UFUNCTION()
	void OnRep_ItemPossession(FItemPossessionInfo PrevPossession);

	void Broadcast_PossessionChanged(FItemPossessionInfo PrevPossession);

	/**
	 * Is this item instanced now (visible in the world and can interact with it).
	 */
	UPROPERTY(BlueprintGetter = GetIsInstanced, ReplicatedUsing = OnRep_IsInstanced, meta = (AllowPrivateAccess), Category = "Item|Condition")
	bool bIsInstanced = true;

	UFUNCTION()
	void OnRep_IsInstanced(bool PrevIsInstanced);

	void Broadcast_ItemInstancedChanged();
};
