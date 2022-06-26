// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "InventoryComponent.h"
#include "ComplexItem.h"
#include "ItemPlace.h"

void AItem::Instance_Implementation()
{
    // ...
}

void AItem::Uninstance_Implementation()
{
    // ...
}

UInventoryComponent* AItem::GetRelatedInventory() const
{
    if(IsValid(PossessingPlace))
    {
        UObject* Possessor = PossessingPlace->GetPossessor();
    
        while(Possessor)
        {
            UInventoryComponent* Inventory = Cast<UInventoryComponent>(Possessor);
            if(IsValid(Inventory))
            {
                return Inventory;
            }
            else
            {
                AComplexItem* ComplexItem = Cast<AComplexItem>(Possessor);
                if(IsValid(ComplexItem->GetPossessingPlace()))
                {
                    Possessor = ComplexItem->GetPossessingPlace()->GetPossessor();
                }
            }
        }
    }
    
    return nullptr;
}

bool AItem::GetSizeForPlace(TSubclassOf<UItemPlace> PlaceClass, FVector& OutSize)
{
    if(IsValid(PlaceClass) && Sizes.Contains(PlaceClass))
    {
        OutSize = Sizes[PlaceClass];
        return true;
    }

    return false;
}

bool AItem::GetLocationInPlace(FVector& OutLocation)
{
    if(IsValid(GetPossessingPlace()))
    {
        OutLocation = LocationInPlace;
        return true;
    }

    return false;
}
