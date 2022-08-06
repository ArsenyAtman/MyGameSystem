// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Item.h"
#include "ItemPlace.h"
#include "ActorWithInventoryInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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

    if(IsValid(GetOwner()) && GetOwner()->Implements<UActorWithInventoryInterface>() && Item->GetRelatedInventory() == this)
    {
        FTransform DropTransform = IActorWithInventoryInterface::Execute_GetDropTransform(GetOwner());
        Item->SetActorTransform(DropTransform);
        Item->RemoveFromPlace();
        IInstanceInterface::Execute_Instance(Item);
        ItemDropped(Item);
        Notify_ItemDropped(Item);
    }
}

void UInventoryComponent::Notify_ItemDropped_Implementation(AItem* Item)
{
    Broadcast_ItemDropped(Item);
}

void UInventoryComponent::Broadcast_ItemDropped(AItem* Item)
{
    OnItemDropped.Broadcast(this, Item);
}
