// Fill out your copyright notice in the Description page of Project Settings.


#include "SlotComponent.h"
#include "Equipment.h"
#include "ItemType.h"
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

void USlotComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate equipment.
	DOREPLIFETIME_CONDITION(USlotComponent, PlacedEquipment, COND_OwnerOnly);
}

void USlotComponent::BeginPlay()
{
	OnComponentActivated;

	UEquipment* Equipment = Cast<UEquipment>(PlacedEquipment.Item);
	if (IsValid(Equipment))
	{
		this->SetChildActorClass(Equipment->ActorClass);
	}
	else
	{
		this->SetChildActorClass(nullptr);
	}

	Super::BeginPlay();

	InitializeItem();
}

TArray<FItemStruct> USlotComponent::GetItems_Implementation() const
{
	TArray<FItemStruct> Items;
	Items.Add(PlacedEquipment);
	return Items;
}

bool USlotComponent::AddItem_Implementation(FItemStruct Item, int InPlace)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		UEquipment* Equipment = Cast<UEquipment>(Item.Item);
		if (IsValid(Equipment))
		{
			if (!IsValid(PlacedEquipment.Item) && Equipment->Type.Get()->IsChildOf(this->Type))
			{
				if (IsValid(IItemContainerInterface::Execute_RemoveItem(Item.OwningContainer, Item.Place).Item))
				{
					PlacedEquipment = FItemStruct(Equipment, this, 0);
					this->DestroyChildActor();
					this->SetChildActorClass(Equipment->ActorClass);
					this->CreateChildActor();
					OnSlotUpdate.Broadcast();
					return true;
				}
			}
		}
	}
	return false;
}

FItemStruct USlotComponent::RemoveItem_Implementation(int FromPlace)
{
	FItemStruct RemovedItem = PlacedEquipment;
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		PlacedEquipment = FItemStruct();
		this->DestroyChildActor();
		this->SetChildActorClass(nullptr);
		OnSlotUpdate.Broadcast();
	}
	return RemovedItem;
}

void USlotComponent::InitializeItem()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(PlacedEquipment.Item))
		{
			PlacedEquipment.OwningContainer = this;
			PlacedEquipment.Place = 0;
		}
		else
		{
			PlacedEquipment = FItemStruct();
		}
	}
}

void USlotComponent::Update_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		OnSlotUpdate.Broadcast();
		UpdateClient();
	}
}

void USlotComponent::UpdateClient_Implementation()
{
	OnSlotUpdate.Broadcast();
}

void USlotComponent::OnRep_PlacedEquipment()
{
	OnSlotUpdate.Broadcast();
}