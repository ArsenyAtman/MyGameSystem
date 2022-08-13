// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

#include "Item.h"
#include "ItemPlace.h"
#include "ActorWithInventoryInterface.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

bool UInventoryComponent::AddItem_Implementation(AItem* Item)
{
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

TArray<AItem*> UInventoryComponent::FindItemsByClass_Implementation(TSubclassOf<AItem> ItemClass) const
{
    TArray<AItem*> FoundItems;

    TArray<UItemPlace*> Places = IComplexStorageInterface::Execute_GetPlaces(this);
    for (UItemPlace* Place : Places)
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

TArray<UItemPlace*> UInventoryComponent::GetPlaces_Implementation() const
{
    TArray<UItemPlace*> FoundPlaces;
    bool bIncludeComponentsFromChildActors = true;
	if(IsValid(GetOwner()))
	{
		GetOwner()->GetComponents(FoundPlaces, bIncludeComponentsFromChildActors);
	}
    return FoundPlaces;
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

void UInventoryComponent::Notify_ItemDropped_Implementation(AItem* Item)
{
    ItemDropped(Item);
    Broadcast_ItemDropped(Item);
}

void UInventoryComponent::Broadcast_ItemDropped(AItem* Item)
{
    OnItemDropped.Broadcast(this, Item);
}
