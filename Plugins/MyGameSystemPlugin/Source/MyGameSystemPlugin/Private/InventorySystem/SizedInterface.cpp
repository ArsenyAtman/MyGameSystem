// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/SizedInterface.h"

FBox2D ISizedInterface::GetBox() const
{
    FVector2D Location = ISizedInterface::Execute_GetInventoryLocation(Cast<UObject>(this));
    FVector2D Size = ISizedInterface::Execute_GetInventorySize(Cast<UObject>(this));
    return FBox2D(Location, Location + Size);
}

FBox2D ISizedInterface::GetBoxForPlace(FVector2D Location, UItemPlace* Place) const
{
    FVector2D Size = ISizedInterface::Execute_GetInventorySizeForPlace(Cast<UObject>(this), Place);
    return FBox2D(Location, Location + Size);
}