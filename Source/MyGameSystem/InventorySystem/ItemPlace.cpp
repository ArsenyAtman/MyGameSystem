// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemPlace.h"

#include "Item.h"
#include "ComplexItem.h"
#include "InventoryComponent.h"
#include "ActorWithInventoryInterface.h"
#include "Math\Box2D.h"
#include "ItemLocator.h"
#include "MyGameSystem/ArrayFunctionLibrary/ArrayFunctionLibrary.h"
#include "Net/UnrealNetwork.h"

UItemPlace::UItemPlace()
{
    SetIsReplicatedByDefault(true);
}

void UItemPlace::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UItemPlace, Items);
}

bool UItemPlace::AddItem_Implementation(AItem* Item)
{
    if(GetOwner()->HasAuthority() == false)
    {
        return false;
    }

    if (Item->Implements<UStorageInterface>())
    {
        if (IStorageInterface::Execute_CheckItemPossession(Item, Cast<AItem>(this->GetPossessor())))
        {
            return false;
        }
    }

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
        if (ItemInPlace->Implements<UStorageInterface>())
        {
            if (IStorageInterface::Execute_AddItem(ItemInPlace, Item))
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
        if (IsValid(Item) && Item->GetClass()->IsChildOf(ItemClass))
        {
            FoundItems.Add(Item);
        }

        AComplexItem* ComplexItem = Cast<AComplexItem>(Item);
        if (IsValid(ComplexItem))
        {
            FoundItems.Append(IStorageInterface::Execute_FindItemsByClass(ComplexItem, ItemClass));
        }
    }

    FoundItems.Remove(nullptr);

    return FoundItems;
}

bool UItemPlace::CheckItemPossession_Implementation(AItem* Item) const
{
    if (IsValid(Item))
    {
        for (AItem* ItemInPlace : Items)
        {
            bool bBelongs = false;
            if (ItemInPlace->Implements<UStorageInterface>())
            {
                bBelongs = IStorageInterface::Execute_CheckItemPossession(ItemInPlace, Item);
            }
            else
            {
                bBelongs = ItemInPlace == Item;
            }

            if (bBelongs)
            {
                return true;
            }
        }
    }

    return false;
}

void UItemPlace::SetIsInstanced_Implementation(bool bNewIsInstanced)
{
    if (bIsInstancing)
    {
        for (AItem* Item : Items)
        {
            if (IsValid(Item))
            {
                IInstanceInterface::Execute_SetIsInstanced(Item, bNewIsInstanced);
            }
        }
    }
}

UInventoryComponent* UItemPlace::GetRelatedInventory() const
{
    AItem* OwningItem = Cast<AComplexItem>(GetOwner());
    if (IsValid(OwningItem))
    {
        return OwningItem->GetRelatedInventory();
    }

    if (GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        return IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
    }

    return nullptr;
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
    if(GetOwner()->HasAuthority() == false)
    {
        return false;
    }

    if (NewItem->Implements<UStorageInterface>())
    {
        if (IStorageInterface::Execute_CheckItemPossession(NewItem, Cast<AItem>(this->GetPossessor())))
        {
            return false;
        }
    }

    FBox2D PlaceBox = this->GetBox();
    FBox2D NewItemBox = NewItem->GetBoxForPlace(NewItemPosition, this);
    if(NewItemBox.Min == NewItemBox.Max)
    {
        return false;
    }
    
    // Correction for comparsions (IsInside and Intersect)
    FVector2D Correction = FVector2D(0.1, 0.1);
    NewItemBox.Min += Correction;
    NewItemBox.Max -= Correction;

    if (PlaceBox.IsInside(NewItemBox))
    {
        for (const AItem* Item : Items)
        {
            FBox2D ItemBox = Item->GetBox();
            if (NewItemBox.Intersect(ItemBox) && NewItem != Item)
            {
                return false;
            }
        }

        Items.Add(NewItem);
        NewItem->RemoveFromCurrentPlace();
        NewItem->ChangePossession(FItemPossessionInfo(this, NewItemPosition));

        IInstanceInterface::Execute_SetIsInstanced(NewItem, bIsInstancing);

        ItemPlaced(NewItem);
        Broadcast_ItemPlaced(NewItem);

        return true;
    }

    return false;
}

bool UItemPlace::RemoveItem(AItem* Item)
{
    if(GetOwner()->HasAuthority() == false)
    {
        return false;
    }

    if (Items.Find(Item) != INDEX_NONE)
    {
        Items.Remove(Item);
        Item->ChangePossession(FItemPossessionInfo());

        ItemRemoved(Item);
        Broadcast_ItemRemoved(Item);

        return true;
    }

    return false;
}

void UItemPlace::SetIsInstancing(bool bNewIsInstancing)
{
    if(GetOwner()->HasAuthority() == false)
    {
        return;
    }

    bIsInstancing = bNewIsInstancing;

    IInstanceInterface::Execute_SetIsInstanced(this, bIsInstancing);
}

FTransform UItemPlace::GetRelativeTransformForItem(AItem* Item)
{
    if (IsValid(ItemLocator))
    {
        return ItemLocator->GetItemRelativeTransform(this, Item);
    }

    return FTransform::Identity;
}

void UItemPlace::OnRep_Items(const TArray<AItem*>& PrevItems)
{
    BroadcastChange_Items(PrevItems);
}

void UItemPlace::BroadcastChange_Items(const TArray<AItem*>& PrevItems)
{
    const TArray<AItem*>& CurrentItems = Items;

    TArray<AItem*> AddedItems = CurrentItems.FilterByPredicate([PrevItems](AItem* const& Item){ return PrevItems.Find(Item) == INDEX_NONE; });
	TArray<AItem*> RemovedItems = PrevItems.FilterByPredicate([CurrentItems](AItem* const& Item){ return CurrentItems.Find(Item) == INDEX_NONE; });

	for(AItem* Item : AddedItems)
	{
        ItemPlaced(Item);
		Broadcast_ItemPlaced(Item);
	}

	for(AItem* Item : RemovedItems)
	{
        ItemRemoved(Item);
		Broadcast_ItemRemoved(Item);
	}
}

void UItemPlace::Broadcast_ItemPlaced(AItem* Item)
{
    OnPlaced.Broadcast(this, Item);
}

void UItemPlace::Broadcast_ItemRemoved(AItem* Item)
{
    OnRemoved.Broadcast(this, Item);
}
