// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "InventoryComponent.h"
#include "ComplexItem.h"
#include "ItemPlace.h"
#include "ItemResizer.h"

void AItem::Instance_Implementation()
{
    UItemPlace* OwningPlace = GetPossessingPlace();
    if (IsValid(OwningPlace))
    {
        AttachToComponent(OwningPlace, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        SetActorRelativeTransform(OwningPlace->GetRelativeTransformForItem(this));
    }

    ShowView();
}

void AItem::Uninstance_Implementation()
{
    if (IsValid(GetPossessingPlace()))
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    HideView();
}

FVector2D AItem::GetInventorySize() const
{
    if (IsValid(ItemResizer))
    {
        return ItemResizer->GetInventorySize(this, GetPossessingPlace());
    }

    return FVector2D::ZeroVector;
}

UInventoryComponent* AItem::GetRelatedInventory() const
{
    if (IsValid(PossessingPlace))
    {
        UObject* Possessor = PossessingPlace->GetPossessor();

        while (Possessor)
        {
            UInventoryComponent* Inventory = Cast<UInventoryComponent>(Possessor);
            if (IsValid(Inventory))
            {
                return Inventory;
            }
            else
            {
                AItem* Item = Cast<AItem>(Possessor);
                if (IsValid(Item) && IsValid(Item->GetPossessingPlace()))
                {
                    Possessor = Item->GetPossessingPlace()->GetPossessor();
                }
            }
        }
    }

    return nullptr;
}

void AItem::PlaceInPlace(UItemPlace* NewPlace, FVector2D NewLocation)
{
    if (IsValid(GetPossessingPlace()))
    {
        GetPossessingPlace()->RemoveItem(this);
    }

    SetPossessingPlace(NewPlace);
    SetInventoryLocation(NewLocation);
    Placed(NewPlace);
}

void AItem::RemoveFromPlace()
{
    UItemPlace* PrevPlace = GetPossessingPlace();
    SetPossessingPlace(nullptr);
    SetInventoryLocation(FVector2D::ZeroVector);
    Removed(PrevPlace);
}
