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

FVector2D AItem::GetInventorySize() const
{
    return GetInventorySizeForPlace(GetPossessingPlace()->StaticClass());
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

FVector2D AItem::GetInventorySizeForPlace(TSubclassOf<UItemPlace> PlaceClass) const
{
    if(IsValid(PlaceClass) && Sizes.Contains(PlaceClass))
    {
        return Sizes[PlaceClass];
    }

    return FIntPoint::NoneValue;
}

void AItem::PlacedInPlace(UItemPlace* NewPlace, FVector2D NewLocation)
{
    if(IsValid(GetPossessingPlace()))
    {
        GetPossessingPlace()->RemoveItem(this);
    }

    SetPossessingPlace(NewPlace);
    SetInventoryLocation(NewLocation);
    Placed(NewPlace);
}

void AItem::RemovedFromPlace()
{
    UItemPlace* PrevPlace = GetPossessingPlace();
    SetPossessingPlace(nullptr);
    SetInventoryLocation(FVector2D::ZeroVector);
    Removed(PrevPlace);
}
