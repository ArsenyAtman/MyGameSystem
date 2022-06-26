// Fill out your copyright notice in the Description page of Project Settings.


#include "ComplexItem.h"

#include "ItemPlace.h"

void AComplexItem::Instance_Implementation()
{
    Super::Instance_Implementation();

    TArray<UItemPlace*> Places = GetPlaces();
    for (UItemPlace* Place : Places)
    {
        if(IsValid(Place))
        {
            Place->Instance();
        }
    }
}

void AComplexItem::Uninstance_Implementation()
{
    Super::Uninstance_Implementation();

    TArray<UItemPlace*> Places = GetPlaces();
    for (UItemPlace* Place : Places)
    {
        if(IsValid(Place))
        {
            Place->Uninstance();
        }
    }
}

bool AComplexItem::AddItem_Implementation(AItem* Item)
{
    TArray<UItemPlace*> Places = GetPlaces();
    for (UItemPlace* Place : Places)
    {
        if(IsValid(Place) && Place->AddItem(Item))
        {
            return true;
        }
    }

    return false;
}

TArray<AItem*> AComplexItem::FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const
{
    TArray<AItem*> FoundItems;

    TArray<UItemPlace*> Places = GetPlaces();
    for (const UItemPlace* Place : Places)
    {
        if(IsValid(Place))
        {
            FoundItems.Append(Place->FindItemsByClass(ItemClass));
        }
    }

    FoundItems.Remove(nullptr);

    return FoundItems;
}

TArray<UItemPlace*> AComplexItem::GetPlaces_Implementation() const
{
    TArray<UItemPlace*> FoundPlaces;
    bool bIncludeComponentsFromChildActors = true;
    GetComponents(FoundPlaces, bIncludeComponentsFromChildActors);
    return FoundPlaces;
}
