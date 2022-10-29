// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/ItemLocator_Grid.h"

#include "InventorySystem/SizedInterface.h"
#include "InventorySystem/Item.h"

FTransform UItemLocator_Grid::GetItemRelativeTransform_Implementation(UItemPlace* OwningPlace, AItem* Item) const
{
    float Y = GridStepSize * ISizedInterface::Execute_GetInventoryLocation(Item).X;
    float Z = GridStepSize * ISizedInterface::Execute_GetInventoryLocation(Item).Y;
    FVector RelativeLocation = FVector(0.0f, Y, Z);
    FTransform ResultTransform;
    ResultTransform.SetLocation(RelativeLocation);
    ResultTransform.SetRotation(FQuat(ItemsRelativeRotation));
    return ResultTransform;
}