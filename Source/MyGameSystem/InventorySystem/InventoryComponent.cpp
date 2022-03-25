// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "SlotComponent.h"
#include "Item.h"
#include "PickableInterface.h"
#include "Equipment.h"
#include "Components/CapsuleComponent.h"
#include "ItemType.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	//SetIsReplicated(true);
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InventorySize = 8;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate items.
	DOREPLIFETIME(UInventoryComponent, Items);

	// Replicate slots.
	DOREPLIFETIME(UInventoryComponent, Slots);

	//Replicate the opened container.
	DOREPLIFETIME_CONDITION(UInventoryComponent, OpenedContainer, COND_OwnerOnly);
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (this->GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		Slots = FindAllSlots();

	}

	InitializeItems();

}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FItemStruct> UInventoryComponent::GetItemsOfSlots() const
{
	TArray<FItemStruct> ItemsOfSlots = TArray<FItemStruct>();

	for (USlotComponent* Slot : Slots)
	{
		FItemStruct ItemOfSlot = IItemContainerInterface::Execute_GetItems(Slot)[0];
		if (IsValid(ItemOfSlot.Item))
		{
			ItemsOfSlots.Add(ItemOfSlot);
		}
	}

	return ItemsOfSlots;
}

int UInventoryComponent::GetItemsCountOfClass(UItem* ItemClass) const
{
	int ItemsCount = 0;
	for (FItemStruct ItemInInventory : Items)
	{
		if (IsValid(ItemInInventory.Item))
		{
			if (ItemInInventory.Item == ItemClass)
			{
				ItemsCount++;
			}
		}
	}
	return ItemsCount;
}

int UInventoryComponent::GetItemsCountOfType(TSubclassOf<UItemType> ItemType) const
{
	int ItemsCount = 0;
	for (FItemStruct ItemInInventory : Items)
	{
		if (IsValid(ItemInInventory.Item))
		{
			if (ItemInInventory.Item->Type->IsChildOf(ItemType))
			{
				ItemsCount++;
			}
		}
	}
	return ItemsCount;
}

bool UInventoryComponent::RemoveItemOfClass(UItem* ItemClass)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		for (int i = 0; i < InventorySize; i++)
		{
			if (IsValid(Items[i].Item))
			{
				if (Items[i].Item == ItemClass)
				{
					Items[i] = FItemStruct();
					OnItemsUpdate.Broadcast();
					return true;
				}
			}
		}
	}
	return false;
}

bool UInventoryComponent::RemoveItemOfType(TSubclassOf<UItemType> ItemType)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		for (int i = 0; i < InventorySize; i++)
		{
			if (IsValid(Items[i].Item))
			{
				if (Items[i].Item->Type->IsChildOf(ItemType))
				{
					Items[i] = FItemStruct();
					OnItemsUpdate.Broadcast();
					return true;
				}
			}
		}
	}
	return false;
}

bool UInventoryComponent::Pickup(UItem* Item)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		UEquipment* Equipment = Cast<UEquipment>(Item);
		if (IsValid(Equipment))
		{
			if (IsValid(Slots[0]))
			{
				if (IItemContainerInterface::Execute_GetItems(Slots[0]).IsValidIndex(0))
				{
					this->Drop(IItemContainerInterface::Execute_GetItems(Slots[0])[0]);
				}

				return IItemContainerInterface::Execute_AddItem(Slots[0], FItemStruct(Item), -1);
			}
		}
		else
		{
			return IItemContainerInterface::Execute_AddItem(this, FItemStruct(Item), -1);
		}
	}
	return false;
}

bool UInventoryComponent::AddItem_Implementation(FItemStruct Item, int InPlace)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (InPlace == -1)
		{
			if (IsValid(Item.Item))
			{
				for (int i = 0; i < InventorySize; i++)
				{
					if (!IsValid(Items[i].Item))
					{
						Item.OwningContainer = this;
						Item.Place = i;
						Items[i] = Item;
						OnItemsUpdate.Broadcast();
						return true;
					}
				}
			}
		}
		else
		{
			if (InPlace >= 0 && InPlace < InventorySize && Items[InPlace] == FItemStruct())
			{
				if (IsValid(Item.OwningContainer))
				{
					if (IsValid(IItemContainerInterface::Execute_RemoveItem(Item.OwningContainer, Item.Place).Item))
					{
						Item.OwningContainer = this;
						Item.Place = InPlace;
						Items[InPlace] = Item;
						OnItemsUpdate.Broadcast();
						return true;
					}
				}
			}
		}
	}
	return false;
}

FItemStruct UInventoryComponent::RemoveItem_Implementation(int FromPlace)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (FromPlace != INDEX_NONE)
		{
			FItemStruct RemovedItem = Items[FromPlace];
			Items[FromPlace] = FItemStruct();
			OnItemsUpdate.Broadcast();
			return RemovedItem;
		}
	}
	return FItemStruct();
}

void UInventoryComponent::Update_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		OnItemsUpdate.Broadcast();
		OnSlotsUpdate.Broadcast();
		UpdateClient();
	}
}

void UInventoryComponent::UpdateClient_Implementation()
{
	OnItemsUpdate.Broadcast();
	OnSlotsUpdate.Broadcast();
}

bool UInventoryComponent::MoveItem_Validate(UObject* FromContainer, int FromPosition, UObject* ToContainer, int ToPosition)
{
	if (IsValid(FromContainer) && IsValid(ToContainer))
	{
		if (CheckContainer(FromContainer) && CheckContainer(ToContainer))
		{
			if (IItemContainerInterface::Execute_GetItems(FromContainer).IsValidIndex(FromPosition) && IItemContainerInterface::Execute_GetItems(ToContainer).IsValidIndex(ToPosition))
			{
				if (IsValid(IItemContainerInterface::Execute_GetItems(FromContainer)[FromPosition].Item))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void UInventoryComponent::MoveItem_Implementation(UObject* FromContainer, int FromPosition, UObject* ToContainer, int ToPosition)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (!(FromContainer == ToContainer && FromPosition == ToPosition))
		{
			FItemStruct MovingItem = IItemContainerInterface::Execute_GetItems(FromContainer)[FromPosition];
			if (!IItemContainerInterface::Execute_AddItem(ToContainer, MovingItem, ToPosition))
			{
				IItemContainerInterface::Execute_Update(FromContainer);
			}
		}
		else
		{
			IItemContainerInterface::Execute_Update(FromContainer);
		}
	}
}

bool UInventoryComponent::DropItem_Validate(UObject* FromContainer, int FromPosition)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(FromContainer))
		{
			if (CheckContainer(FromContainer))
			{
				if (IItemContainerInterface::Execute_GetItems(FromContainer).IsValidIndex(FromPosition))
				{
					if (IsValid(IItemContainerInterface::Execute_GetItems(FromContainer)[FromPosition].Item))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void UInventoryComponent::DropItem_Implementation(UObject* FromContainer, int FromPosition)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		FItemStruct DroppingItem = IItemContainerInterface::Execute_GetItems(FromContainer)[FromPosition];
		this->Drop(DroppingItem);
	}
}

void UInventoryComponent::Drop(FItemStruct Item)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		if (IsValid(Item.Item) && IsValid(Item.OwningContainer))
		{
			if (IsValid(IItemContainerInterface::Execute_RemoveItem(Item.OwningContainer, Item.Place).Item))
			{
				FVector SpawnLocation = GetOwner()->GetActorLocation();
				ACharacter* Character = Cast<ACharacter>(GetOwner());
				if (IsValid(Character))
				{
					SpawnLocation = Character->GetCapsuleComponent()->GetComponentLocation() + FVector(0.0f, 0.0f, -1.0f) * Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
				}

				FTransform SpawnTransform = FTransform(GetOwner()->GetActorRotation(), SpawnLocation, FVector::OneVector);
				AActor* ItemActor = GetWorld()->SpawnActor<AActor>(PickableClass, SpawnTransform);
				if (ItemActor->Implements<UPickableInterface>())
				{
					IPickableInterface::Execute_SetItem(ItemActor, Item.Item);
				}
				else
				{
					ItemActor->Destroy();
				}

				OnItemsUpdate.Broadcast();
			}
		}
	}
}

TArray<USlotComponent*> UInventoryComponent::FindAllSlots()
{
	TArray<USlotComponent*> FoundSlots;
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		ACharacter* Character = this->GetOwner<ACharacter>();
		if (IsValid(Character))
		{
			TArray<UActorComponent*> FoundComponents;
			Character->GetComponents(USlotComponent::StaticClass(), FoundComponents, true);
			for (int i = 0; i < FoundComponents.Num(); i++)
			{
				USlotComponent* Slot = Cast<USlotComponent>(FoundComponents[i]);
				if (IsValid(Slot))
				{
					FoundSlots.Add(Slot);
				}
			}
		}
	}
	return FoundSlots;
}

void UInventoryComponent::InitializeItems()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		for (int i = 0; i < InventorySize; i++)
		{
			if (!Items.IsValidIndex(i))
			{
				Items.Add(FItemStruct());
			}
			else
			{
				Items[i] = FItemStruct(Items[i].Item, this, i);
			}
		}

		while (Items.Num() > InventorySize)
		{
			Items.RemoveAt(Items.Num() - 1);
		}
	}
}

void UInventoryComponent::OnRep_Items()
{
	OnItemsUpdate.Broadcast();
}

void UInventoryComponent::OnRep_Slots()
{
	OnSlotsUpdate.Broadcast();
}

bool UInventoryComponent::CheckContainer(UObject* Container)
{
	return Container == this || Container == OpenedContainer || Slots.Contains(Container);
}

void UInventoryComponent::OpenContainer(UObject* Container)
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		OpenedContainer = Container;
		OnContainerOpened.Broadcast();
	}
}

void UInventoryComponent::CloseContainer_Implementation()
{
	if (GetOwnerRole() == ENetRole::ROLE_Authority)
	{
		OpenedContainer = nullptr;
		OnContainerClosed.Broadcast();
	}
}

int UInventoryComponent::GetItemsCount()
{
	int Count = 0;

	for (FItemStruct Item : Items)
	{
		if (IsValid(Item.Item))
		{
			Count++;
		}
	}

	return Count;
}

void UInventoryComponent::OnRep_Container()
{
	if (IsValid(OpenedContainer))
	{
		OnContainerOpened.Broadcast();
	}
	else
	{
		OnContainerClosed.Broadcast();
	}
}