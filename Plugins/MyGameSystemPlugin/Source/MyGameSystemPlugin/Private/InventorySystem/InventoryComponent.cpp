// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/InventoryComponent.h"

#include "InventorySystem/Item.h"
#include "InventorySystem/ItemPlace.h"
#include "InventorySystem/ActorWithInventoryInterface.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UInventoryComponent, ItemPlaces);
}

bool UInventoryComponent::AddItem_Implementation(AItem* Item)
{
    for (UItemPlace* Place : ItemPlaces)
    {
        if(IsValid(Place) && IStorageInterface::Execute_AddItem(Place, Item))
        {
            return true;
        }
    }

    return false;
}

TArray<AItem*> UInventoryComponent::FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const
{
    TArray<AItem*> FoundItems;

    for (UItemPlace* Place : ItemPlaces)
    {
        if(IsValid(Place))
        {
            FoundItems.Append(IStorageInterface::Execute_FindItemsByClass(Place, ItemClass));
        }
    }

    FoundItems.Remove(nullptr);

    return FoundItems;
}

bool UInventoryComponent::CheckItemPossession_Implementation(AItem* Item) const
{
    if (IsValid(Item))
    {
        for (UItemPlace* Place : ItemPlaces)
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

bool UInventoryComponent::AddPlace(UItemPlace* ItemPlace)
{
    if (GetOwner()->HasAuthority())
    {
        int32 Result = ItemPlaces.AddUnique(ItemPlace);
        if (Result != INDEX_NONE)
        {
            Broadcast_OnItemPlaceAdded(ItemPlace);
            return true;
        }
    }

    return false;
}

bool UInventoryComponent::RemovePlace(UItemPlace* ItemPlace)
{
    if (GetOwner()->HasAuthority())
    {
        int32 Result = ItemPlaces.Remove(ItemPlace);
        if (Result != INDEX_NONE)
        {
            Broadcast_OnItemPlaceRemoved(ItemPlace);
            return true;
        }
    }

    return false;
}

void UInventoryComponent::DropItem(AItem* Item)
{
    if(GetOwner()->HasAuthority() == false)
    {
        return;
    }

    if(IsValid(GetOwner()) && GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        FTransform DropTransform = IActorWithInventoryInterface::Execute_GetDropTransform(GetOwner());
        Item->SetActorTransform(DropTransform);
        Item->GetPossessingPlace()->RemoveItem(Item);
        IInstanceInterface::Execute_SetIsInstanced(Item, true);

        Notify_ItemDropped(Item);
    }
}

void UInventoryComponent::OnRep_Places(const TArray<UItemPlace*>& PrevItemPlaces)
{
    BroadcastChange_Places(PrevItemPlaces);
}

void UInventoryComponent::BroadcastChange_Places(const TArray<UItemPlace*>& PrevItemPlaces)
{
    const TArray<UItemPlace*>& CurrentItemPlaces = ItemPlaces;
	TArray<UItemPlace*> AddedItemPlaces = CurrentItemPlaces.FilterByPredicate([PrevItemPlaces](UItemPlace* const& ItemPlace){ return PrevItemPlaces.Find(ItemPlace) == INDEX_NONE; });
	TArray<UItemPlace*> RemovedItemPlaces = PrevItemPlaces.FilterByPredicate([CurrentItemPlaces](UItemPlace* const& ItemPlace){ return CurrentItemPlaces.Find(ItemPlace) == INDEX_NONE; });

	for(UItemPlace* ItemPlace : AddedItemPlaces)
	{
		Broadcast_OnItemPlaceAdded(ItemPlace);
	}

	for(UItemPlace* ItemPlace : RemovedItemPlaces)
	{
		Broadcast_OnItemPlaceRemoved(ItemPlace);
	}
}

void UInventoryComponent::Broadcast_OnItemPlaceAdded(UItemPlace* ItemPlace)
{
    OnPlaceAdded.Broadcast(this, ItemPlace);
}

void UInventoryComponent::Broadcast_OnItemPlaceRemoved(UItemPlace* ItemPlace)
{
    OnPlaceRemoved.Broadcast(this, ItemPlace);
}

void UInventoryComponent::Notify_ItemDropped_Implementation(AItem* Item)
{
    ItemDropped(Item);
    Broadcast_ItemDropped(Item);
}

void UInventoryComponent::Broadcast_ItemDropped(AItem* Item)
{
    OnItemDropped.Broadcast(this, Item);
}
