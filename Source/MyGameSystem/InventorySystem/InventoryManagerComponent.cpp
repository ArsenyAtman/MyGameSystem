// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"

#include "Item.h"
#include "ItemPlace.h"
#include "InventoryComponent.h"
#include "ActorWithInventoryInterface.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInventoryManagerComponent::ChangeItemPossession_Implementation(AItem* Item, FItemPossessionInfo NewPossessionInfo)
{
	if (!IsValid(Item))
	{
		return;
	}

	if (Item->GetRelatedInventory() == GetInventoryOfOwner() && NewPossessionInfo.PossessingPlace == nullptr)
	{
		DropItem(Item);
	}
	else if (Item->GetRelatedInventory() == nullptr && IsValid(NewPossessionInfo.PossessingPlace) && NewPossessionInfo.PossessingPlace->GetRelatedInventory() == GetInventoryOfOwner())
	{
		PickupItem(Item);
	}
	else if (IsValid(Item) && IsValid(NewPossessionInfo.PossessingPlace) && (Item->GetRelatedInventory() == Item->GetRelatedInventory() || Connections.Find(Item->GetRelatedInventory()) != INDEX_NONE))
	{
		NewPossessionInfo.PossessingPlace->PlaceItem(Item, NewPossessionInfo.InventoryLocation);
	}
}

void UInventoryManagerComponent::PickupItem_Implementation(AItem* Item)
{
	if (GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        UInventoryComponent* MainInventory = IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
		MainInventory->AddItem(Item);
    }
}

void UInventoryManagerComponent::DropItem_Implementation(AItem* Item)
{
	if (GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        UInventoryComponent* MainInventory = IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
		MainInventory->DropItem(Item);
    }
}

bool UInventoryManagerComponent::Connect(UInventoryComponent* InventoryComponent)
{
	if (IsValid(InventoryComponent))
	{
		return Connections.Add(InventoryComponent) != INDEX_NONE;
	}
	
	return false;
}

bool UInventoryManagerComponent::Disconnect(UInventoryComponent* InventoryComponent)
{
	if (IsValid(InventoryComponent))
	{
		return Connections.Remove(InventoryComponent) > 0;
	}

	return false;
}

UInventoryComponent* UInventoryManagerComponent::GetInventoryOfOwner() const
{
	if (GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        return IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
    }

	return nullptr;
}

bool UInventoryManagerComponent::ChangeItemPossession_Validate(AItem* Item, FItemPossessionInfo NewPossessionInfo)
{
	return IsValid(Item);
}

bool UInventoryManagerComponent::PickupItem_Validate(AItem* Item)
{
	return IsValid(Item) && Item->GetRelatedInventory() == nullptr;
}
	
bool UInventoryManagerComponent::DropItem_Validate(AItem* Item)
{
	return IsValid(Item) && Item->GetRelatedInventory() == GetInventoryOfOwner();
}
