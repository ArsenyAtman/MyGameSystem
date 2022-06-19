// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPlace.h"

#include "Item.h"

void UItemPlace::Instance_Implementation(AItemInstance* ParentInstance)
{
    for (UItem* Item : GetItems())
    {
        Item->Instance(ParentInstance);
    }
}

void UItemPlace::Uninstance_Implementation()
{
    for (UItem* Item : GetItems())
    {
        Item->Uninstance();
    }
}