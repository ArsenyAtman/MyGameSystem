// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPlace.h"

#include "Item.h"
#include "ComplexItem.h"
#include "InventoryComponent.h"
#include "ActorWithInventoryInterface.h"

bool UItemPlace::AddItem_Implementation(AItem* Item)
{
    // ...

    return false;
}

TArray<AItem*> UItemPlace::FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const
{
    TArray<AItem*> FoundItems;

    TArray<AItem*> Items = GetItems();
    for (AItem* Item : Items)
    {
        if(IsValid(Item) && Item->StaticClass()->IsChildOf(ItemClass))
        {
            FoundItems.Add(Item);
        }

        AComplexItem* ComplexItem = Cast<AComplexItem>(Item);
        if(IsValid(ComplexItem))
        {
            FoundItems.Append(ComplexItem->FindItemsByClass(ItemClass));
        }
    }

    FoundItems.Remove(nullptr);

    return FoundItems;
}

void UItemPlace::Instance_Implementation()
{
    TArray<AItem*> Items = GetItems();
    for (AItem* Item : Items)
    {
        if(IsValid(Item))
        {
            Item->Instance();
        }
    }
}

void UItemPlace::Uninstance_Implementation()
{
    TArray<AItem*> Items = GetItems();
    for (AItem* Item : Items)
    {
        if(IsValid(Item))
        {
            Item->Uninstance();
        }
    }
}

UObject* UItemPlace::GetPossessor() const
{
    AActor* Owner = GetOwner();

    AComplexItem* ComplexItem = Cast<AComplexItem>(Owner);
    if(IsValid(ComplexItem))
    {
        return ComplexItem;
    }

    if(Owner->Implements<UActorWithInventoryInterface>())
    {
        return IActorWithInventoryInterface::Execute_GetInventoryComponent(Owner);
    }

    return nullptr;
}
