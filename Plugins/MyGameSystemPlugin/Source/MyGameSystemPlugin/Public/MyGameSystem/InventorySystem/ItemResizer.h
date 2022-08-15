// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemResizer.generated.h"

class AItem;
class UItemPlace;

/**
 * Controller of size for items.
 */
UCLASS(DefaultToInstanced, EditInlineNew, BlueprintType, Blueprintable)
class MYGAMESYSTEMPLUGIN_API UItemResizer : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Get the current size.
	 * @param OwningItem - The item that owns this resizer.
	 * @param PossessingPlace - The place that owns the item.
	 * @return Size.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ItemResizer")
	FVector2D GetInventorySize(const AItem* OwningItem, const UItemPlace* PossessingPlace) const;
	virtual FVector2D GetInventorySize_Implementation(const AItem* OwningItem, const UItemPlace* PossessingPlace) const { return FVector2D::ZeroVector; }

};
