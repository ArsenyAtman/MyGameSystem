// Fill out your copyright notice in the Description page of Project Settings.


#include "Container.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InventoryItem.h"

bool UContainer::Add_Implementation(UInventoryItem* Item, FPoint InLocation)
{

	if (CheckOnCompability(Item) && Items.Num() < MaxCountOfItems)
	{
		Item->RemoveSelf();
		Items.Add(Item);
		Item->Place(FPoint(), this);

		if (bIsInstancing)
		{
			//Item->SpawnProjection(UniversalMesh);
		}

		return true;
	}

	return false;
}

UInventoryItem* UContainer::Remove_Implementation(UInventoryItem* Item)
{
	if (Items.Num() > 0)
	{
		UInventoryItem* ItemToRemove = Items.Pop();
		UKismetSystemLibrary::PrintString(GetWorld(), FString::FromInt(Items.Num()), true, true, FLinearColor::Yellow);
		ItemToRemove->DestroyInstance();
		return ItemToRemove;
	}
	return nullptr;
}

UInventoryItem* UContainer::GetLastItem()
{
	if (Items.Num() > 0)
	{
		return Items[Items.Num() - 1];
	}
	else
	{
		return nullptr;
	}
}

int UContainer::Num()
{
	return Items.Num();
}

TArray<UInventoryItem*> UContainer::GetItems_Implementation()
{
	return Items;
}