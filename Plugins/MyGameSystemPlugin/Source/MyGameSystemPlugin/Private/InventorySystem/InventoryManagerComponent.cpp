// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystem/InventoryManagerComponent.h"

#include "InventorySystem/Item.h"
#include "InventorySystem/ComplexItem.h"
#include "InventorySystem/StackableItem.h"
#include "InventorySystem/ItemPlace.h"
#include "InventorySystem/InventoryComponent.h"
#include "InventorySystem/ActorWithInventoryInterface.h"
#include "Net/UnrealNetwork.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UInventoryManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryManagerComponent, Connections);
}

void UInventoryManagerComponent::ChangeItemPossession_Implementation(AItem* Item, FItemPossessionInfo NewPossessionInfo)
{
	if (!IsValid(Item))
	{
		return;
	}

	if (NewPossessionInfo.PossessingPlace == nullptr && (Item->GetRelatedInventory() == GetInventoryOfOwner() || Connections.Find(Item->GetRelatedInventory()) != INDEX_NONE))
	{
		DropItem(Item);
	}
	else if (Item->GetRelatedInventory() == nullptr && IsValid(NewPossessionInfo.PossessingPlace) && NewPossessionInfo.PossessingPlace->GetRelatedInventory() == GetInventoryOfOwner())
	{
		PickupItem(Item);
	}
	else if (IsValid(NewPossessionInfo.PossessingPlace) && (Item->GetRelatedInventory() == GetInventoryOfOwner() || Connections.Find(Item->GetRelatedInventory()) != INDEX_NONE))
	{
		NewPossessionInfo.PossessingPlace->PlaceItem(Item, NewPossessionInfo.InventoryLocation);
	}
}

void UInventoryManagerComponent::PickupItem_Implementation(AItem* Item)
{
	if (IsValid(Item) && Item->GetRelatedInventory() == nullptr && GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        UInventoryComponent* MainInventory = IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
		IStorageInterface::Execute_AddItem(MainInventory, Item);
    }
}

void UInventoryManagerComponent::DropItem_Implementation(AItem* Item)
{
	if (IsValid(Item) && IsValid(Item->GetRelatedInventory()) && GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        UInventoryComponent* MainInventory = IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
		MainInventory->DropItem(Item);
    }
}

void UInventoryManagerComponent::AddItemToComplexItem_Implementation(AItem* Item, AComplexItem* ComplexItem)
{
	if (Item != ComplexItem && IsValid(Item) && IsValid(ComplexItem))
	{
		IStorageInterface::Execute_AddItem(ComplexItem, Item);
	}
}

void UInventoryManagerComponent::MergeStackableItems_Implementation(AStackableItem* ReceivingItem, AStackableItem* IncomingItem)
{
	if (ReceivingItem != IncomingItem && IsValid(ReceivingItem) && IsValid(IncomingItem))
	{
		IStorageInterface::Execute_AddItem(ReceivingItem, IncomingItem);
	}
}

void UInventoryManagerComponent::SplitStackableItem_Implementation(AStackableItem* ItemToSplit, int32 CountToTake, FItemPossessionInfo PossessionInfo)
{
	if (IsValid(ItemToSplit) && CountToTake > 0)
	{
		AStackableItem* NewItem = ItemToSplit->Split(CountToTake);
		if (IsValid(NewItem) == false)
		{
			return;
		}

		bool bPlaceResult = PossessionInfo.PossessingPlace->PlaceItem(NewItem, PossessionInfo.InventoryLocation);
		if(bPlaceResult == false)
		{
			IStorageInterface::Execute_AddItem(ItemToSplit, NewItem);
		}
	}
}

void UInventoryManagerComponent::SplitAndMergeStackableItem_Implementation(AStackableItem* ReceivingItem, AStackableItem* ItemToSplit, int32 CountToTake)
{
	if (ReceivingItem != ItemToSplit && IsValid(ReceivingItem) && IsValid(ItemToSplit) && CountToTake > 0)
	{
		AStackableItem* NewItem = ItemToSplit->Split(CountToTake);
		if (IsValid(NewItem) == false)
		{
			return;
		}
		
		bool bAddResult = IStorageInterface::Execute_AddItem(ReceivingItem, NewItem);
		if(bAddResult == false)
		{
			IStorageInterface::Execute_AddItem(ItemToSplit, NewItem);
		}
	}
}

bool UInventoryManagerComponent::Connect(UInventoryComponent* InventoryComponent)
{
	if (GetOwner()->HasAuthority() && IsValid(InventoryComponent) && Connections.Find(InventoryComponent) == INDEX_NONE)
	{
		bool bResult = Connections.Add(InventoryComponent) != INDEX_NONE;
		if (bResult)
		{
			OnInventoryConnected.Broadcast(this, InventoryComponent);
		}
		return bResult;
	}
	
	return false;
}

bool UInventoryManagerComponent::Disconnect(UInventoryComponent* InventoryComponent)
{
	if (GetOwner()->HasAuthority() && IsValid(InventoryComponent))
	{
		bool bResult = Connections.Remove(InventoryComponent) > 0;
		if (bResult)
		{
			OnInventoryDisconnected.Broadcast(this, InventoryComponent);
		}
		return bResult;
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

void UInventoryManagerComponent::OnRep_Connections(const TArray<UInventoryComponent*>& PrevConnections)
{
	const TArray<UInventoryComponent*>& CurrentConnections = Connections;

	TArray<UInventoryComponent*> AddedConnections = Connections.FilterByPredicate([PrevConnections](UInventoryComponent* const& Connection){ return PrevConnections.Find(Connection) == INDEX_NONE; });
	TArray<UInventoryComponent*> RemovedConnections = PrevConnections.FilterByPredicate([CurrentConnections](UInventoryComponent* const& Connection){ return CurrentConnections.Find(Connection) == INDEX_NONE; });

	for(UInventoryComponent* Connection : AddedConnections)
	{
		OnInventoryConnected.Broadcast(this, Connection);
	}

	for(UInventoryComponent* Connection : RemovedConnections)
	{
		OnInventoryDisconnected.Broadcast(this, Connection);
	}
}
