// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Human.h"
#include "Rifle.h"
#include "ComplexItem.h"
#include "InventoryItem.h"
#include "Slot.h"
#include "PickableInstance.h"
#include "SearchLibrary.h"
#include "SortLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	AHuman* Owner = GetOwner<AHuman>();
	if (IsValid(Owner))
	{
		if (Vest)
		{
			Vest->InstanceItems(Owner->GetMesh());
		}
		
		if (Weapon)
		{
			Weapon->InstanceItems(Owner->GetMesh());
		}
	}
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TArray<APickableInstance*> UInventoryComponent::GetItemsAround(FVector AtLocation)
{
	TArray<AActor*> ActorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes = {
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel7) 
	};
	
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AtLocation, SphereRadius, TraceObjectTypes, APickableInstance::StaticClass(), ActorsToIgnore, OutActors);

	TArray<APickableInstance*> Instances;
	for (int i = 0; i < OutActors.Num(); i++)
	{
		APickableInstance* Instance = Cast<APickableInstance>(OutActors[i]);
		if (IsValid(Instance))
		{
			Instances.Add(Instance);
		}
	}

	return Instances;
}

bool UInventoryComponent::DropItem(UInventoryItem* InventoryItem)
{
	if (InventoryItem)
	{
		FTransform Transform = FTransform::Identity;
		AActor* Owner = GetOwner<AActor>();
		if (IsValid(Owner))
		{
			Transform = Owner->GetTransform();
		}

		UKismetSystemLibrary::PrintString(GetWorld(), "Spawn!");
		InventoryItem->Spawn(Transform, true);
		return true;
	}
	return false;
}

TArray<UInventoryItem*> UInventoryComponent::SearchItemsInInventory(FSearchDelegate Condition)
{
	TArray<UInventoryItem*> Items;

	TArray<USlot*> Slots = this->GetSlots();
	for (int i = 0; i < Slots.Num(); i++)
	{
		USlot* Slot = Slots[i];
		if (Slot)
		{
			if (Slot->GetItems()[0])
			{
				Items.Append(USearchLibrary::SearchItems(Condition, Slot->GetItems()[0]));
			}
		}
	}

	return Items;
}

TArray<USlot*> UInventoryComponent::GetSlots_Implementation()
{
	return { Vest, Belt, Backpack, Helmet };
}