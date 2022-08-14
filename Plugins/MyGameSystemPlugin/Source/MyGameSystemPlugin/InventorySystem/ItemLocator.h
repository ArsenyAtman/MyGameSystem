// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemLocator.generated.h"

class AItem;
class UItemPlace;

/**
 * Controller of items location in places.
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class MYGAMESYSTEM_API UItemLocator : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Get transform for an item in the world.
	 * @param OwningPlace - The place that owns the item.
	 * @param Item - The attached item to the place.
	 * @return Local transform for the item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ItemLocator")
	FTransform GetItemRelativeTransform(UItemPlace* OwningPlace, AItem* Item) const;
	virtual FTransform GetItemRelativeTransform_Implementation(UItemPlace* OwningPlace, AItem* Item) const { return FTransform::Identity; }
};
