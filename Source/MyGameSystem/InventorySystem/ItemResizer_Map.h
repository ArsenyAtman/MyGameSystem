// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemResizer.h"
#include "ItemResizer_Map.generated.h"

class AItem;
class UItemPlace;

/**
 * ItemResizer that store sizes for places in a map.
 */
UCLASS()
class MYGAMESYSTEM_API UItemResizer_Map : public UItemResizer
{
	GENERATED_BODY()

public:

	virtual FVector2D GetInventorySize_Implementation(const AItem* OwningItem, const UItemPlace* PossessingPlace) const override;

private:

	/**
	 * Sizes of the owning item for different places.
	 */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true), Category = "ItemResizer_Map")
	TMap<TSubclassOf<UItemPlace>, FVector2D> Sizes;
	
};
