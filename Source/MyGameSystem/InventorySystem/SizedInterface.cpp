// Fill out your copyright notice in the Description page of Project Settings.


#include "SizedInterface.h"

// Add default functionality here for any ISizedInterface functions that are not pure virtual.

FBox2D ISizedInterface::GetBox() const
{
    FVector2D Location = GetInventoryLocation();
    FVector2D Size = GetInventorySize();
    return FBox2D(Location, Location + Size);
}