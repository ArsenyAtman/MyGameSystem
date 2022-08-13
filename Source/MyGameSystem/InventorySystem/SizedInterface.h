// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Box2D.h"
#include "Math/Vector2D.h"

#include "SizedInterface.generated.h"

class UItemPlace;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USizedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for sizeable objects in the inventory system.
 */
class MYGAMESYSTEM_API ISizedInterface
{
	GENERATED_BODY()

public:

	/**
	 * Get location of this object in inventory.
	 * @return Current location.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SizedInterface")
	FVector2D GetInventoryLocation() const;
	virtual FVector2D GetInventoryLocation_Implementation() const { return FVector2D::ZeroVector; }

	/**
	 * Get size of this object in inventory.
	 * @return Current size.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SizedInterface")
	FVector2D GetInventorySize() const;
	virtual FVector2D GetInventorySize_Implementation() const = 0;

	/**
	 * Get size of this object for a place.
	 * @param Place - A place.
	 * @return Size for the place.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SizedInterface")
	FVector2D GetInventorySizeForPlace(UItemPlace* Place) const;
	virtual FVector2D GetInventorySizeForPlace_Implementation(UItemPlace* Place) const { return FVector2D::ZeroVector; }

	/**
	 * Get the box of this object.
	 * @return Box.
	 */
	virtual FBox2D GetBox() const;

	/**
	 * Get a box pf this object for a place.
	 * @return Box.
	 */
	virtual FBox2D GetBoxForPlace(FVector2D Location, UItemPlace* Place) const;
};
