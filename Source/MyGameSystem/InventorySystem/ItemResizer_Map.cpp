// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemResizer_Map.h"

#include "Item.h"
#include "ItemPlace.h"

FVector2D UItemResizer_Map::GetInventorySize_Implementation(const AItem* OwningItem, const UItemPlace* PossessingPlace) const
{
    if(Sizes.Contains(PossessingPlace->StaticClass()))
    {
        return Sizes[PossessingPlace->StaticClass()];
    }

    return FVector2D::ZeroVector;
}