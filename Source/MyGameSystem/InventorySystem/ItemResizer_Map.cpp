// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemResizer_Map.h"

#include "Item.h"
#include "ItemPlace.h"

FVector2D UItemResizer_Map::GetInventorySize_Implementation(const AItem* OwningItem, const UItemPlace* PossessingPlace) const
{
    if(IsValid(PossessingPlace) && Sizes.Contains(PossessingPlace->GetClass()))
    {
        return Sizes[PossessingPlace->GetClass()];
    }

    return FVector2D::ZeroVector;
}