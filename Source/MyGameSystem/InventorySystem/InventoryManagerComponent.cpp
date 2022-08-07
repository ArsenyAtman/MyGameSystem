// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"

#include "Item.h"
#include "ItemPlace.h"
#include "InventoryComponent.h"
#include "ActorWithInventoryInterface.h"
#include "Net/UnrealNetwork.h"
#include "MyGameSystem/ArrayFunctionLibrary/ArrayFunctionLibrary.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
	if (GetOwner()->Implements<UActorWithInventoryInterface>())
    {
        UInventoryComponent* MainInventory = IActorWithInventoryInterface::Execute_GetInventoryComponent(GetOwner());
		IStorageInterface::Execute_AddItem(MainInventory, Item);
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
	return IsValid(Item) && IsValid(Item->GetRelatedInventory());
}
