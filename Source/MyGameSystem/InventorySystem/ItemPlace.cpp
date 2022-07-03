// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemPlace.h"

#include "Item.h"
#include "ComplexItem.h"
#include "InventoryComponent.h"
#include "ActorWithInventoryInterface.h"
#include "Math\Box2D.h"
#include "ItemLocator.h"

bool UItemPlace::AddItem_Implementation(AItem* Item)
{
    for (int32 X = 0; X < PlaceSize.X; ++X)
    {
        for (int32 Y = 0; Y < PlaceSize.Y; ++Y)
        {
            if (this->PlaceItem(Item, FIntPoint(X, Y)))
            {
                return true;
            }
        }
    }

    for (AItem* ItemInPlace : GetItems())
    {
        AComplexItem* ComplexItemInPlace = Cast<AComplexItem>(ItemInPlace);
        if (IsValid(ComplexItemInPlace))
        {
            if (ComplexItemInPlace->AddItem(Item))
            {
                return true;
            }
        }
    }

    return false;
}

TArray<AItem*> UItemPlace::FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const
{
    TArray<AItem*> FoundItems;

    for (AItem* Item : Items)
    {
        if (IsValid(Item) && Item->StaticClass()->IsChildOf(ItemClass))
        {
            FoundItems.Add(Item);
        }

        AComplexItem* ComplexItem = Cast<AComplexItem>(Item);
        if (IsValid(ComplexItem))
        {
            FoundItems.Append(ComplexItem->FindItemsByClass(ItemClass));
        }
    }

    FoundItems.Remove(nullptr);

    return FoundItems;
}

void UItemPlace::Instance_Implementation()
{
    if (bIsInstancing)
    {
        for (AItem* Item : Items)
        {
            if (IsValid(Item))
            {
                Item->Instance();
            }
        }
    }
}

void UItemPlace::Uninstance_Implementation()
{
    for (AItem* Item : Items)
    {
        if (IsValid(Item))
        {
            Item->Uninstance();
        }
    }
}

UObject* UItemPlace::GetPossessor() const
{
    AActor* Owner = GetOwner();

    AComplexItem* ComplexItem = Cast<AComplexItem>(Owner);
    if (IsValid(ComplexItem))
    {
        return ComplexItem;
    }

    if (Owner->Implements<UActorWithInventoryInterface>())
    {
        return IActorWithInventoryInterface::Execute_GetInventoryComponent(Owner);
    }

    return nullptr;
}

bool UItemPlace::PlaceItem(AItem* NewItem, FVector2D NewItemPosition)
{
    FBox2D NewItemBox = NewItem->GetBox();
    FBox2D PlaceBox = this->GetBox();

    if (PlaceBox.IsInside(NewItemBox))
    {
        for (const AItem* Item : Items)
        {
            FBox2D ItemBox = Item->GetBox();
            if (NewItemBox.Intersect(ItemBox))
            {
                return false;
            }
        }

        Items.Add(NewItem);
        NewItem->PlacedInPlace(this, NewItemPosition);
        ItemPlaced(NewItem);
        return true;
    }

    return false;
}

bool UItemPlace::RemoveItem(AItem* Item)
{
    if (Items.Find(Item) != INDEX_NONE)
    {
        Items.Remove(Item);
        Item->RemovedFromPlace();
        ItemRemoved(Item);
        return true;
    }

    return false;
}

void UItemPlace::SetIsInstancing(bool bNewIsInstancing)
{
    if (bIsInstancing != bNewIsInstancing)
    {
        bIsInstancing = bNewIsInstancing;

        if (bIsInstancing)
        {
            Instance();
        }
        else
        {
            Uninstance();
        }
    }
}

FTransform UItemPlace::GetRelativeTransformForItem(AItem* Item)
{
    if (IsValid(ItemLocator))
    {
        return ItemLocator->GetItemRelativeTransform(this, Item);
    }

    return FTransform::Identity;
}
