// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/ComplexItem.h"

#include "InventorySystem/ItemPlace.h"

void AComplexItem::SetIsInstanced_Implementation(bool bNewIsInstanced)
{
    Super::SetIsInstanced_Implementation(bNewIsInstanced);

    TArray<UItemPlace*> Places = IComplexStorageInterface::Execute_GetPlaces(this);
    for (UItemPlace* Place : Places)
    {
        if(IsValid(Place))
        {
            IInstanceInterface::Execute_SetIsInstanced(Place, bNewIsInstanced);
        }
    }
}

bool AComplexItem::AddItem_Implementation(AItem* Item)
{
    if (Item->Implements<UStorageInterface>())
    {
        if (IStorageInterface::Execute_CheckItemPossession(Item, this))
        {
            return false;
        }
    }

    TArray<UItemPlace*> Places = IComplexStorageInterface::Execute_GetPlaces(this);
    for (UItemPlace* Place : Places)
    {
        if(IsValid(Place) && IStorageInterface::Execute_AddItem(Place, Item))
        {
            return true;
        }
    }

    return false;
}

TArray<AItem*> AComplexItem::FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const
{
    TArray<AItem*> FoundItems;

    TArray<UItemPlace*> Places = IComplexStorageInterface::Execute_GetPlaces(this);
    for (const UItemPlace* Place : Places)
    {
        if(IsValid(Place))
        {
            FoundItems.Append(IStorageInterface::Execute_FindItemsByClass(Place, ItemClass));
        }
    }

    FoundItems.Remove(nullptr);

    return FoundItems;
}

bool AComplexItem::CheckItemPossession_Implementation(AItem* Item) const
{
    if (IsValid(Item))
    {
        if (Item == this)
        {
            return true;
        }

        TArray<UItemPlace*> Places = IComplexStorageInterface::Execute_GetPlaces(this);
        for (UItemPlace* Place : Places)
        {
            bool bBelongs = IStorageInterface::Execute_CheckItemPossession(Place, Item);
            if (bBelongs)
            {
                return true;
            }
        }
    }

    return false;
}

TArray<UItemPlace*> AComplexItem::GetPlaces_Implementation() const
{
    TArray<UItemPlace*> FoundPlaces;
    bool bIncludeComponentsFromChildActors = true;
    GetComponents(FoundPlaces, bIncludeComponentsFromChildActors);
    return FoundPlaces;
}
