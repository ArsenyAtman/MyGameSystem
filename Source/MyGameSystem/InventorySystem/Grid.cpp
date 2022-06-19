// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InventoryItem.h"
#include "ComplexItem.h"

bool UGrid::Add_Implementation(UInventoryItem* Item, FPoint Location)
{
	if (CheckOnCompability(Item))
	{
		if ((Location.X + Item->GetSizeOnGrid().X <= Size.X) && (Location.Y + Item->GetSizeOnGrid().Y <= Size.Y))
		{
			for (int i = 0; i < Items.Num(); i++)
			{
				if (Item != Items[i])
				{
					if (Location.X < Items[i]->GetLocation().X + Items[i]->GetSizeOnGrid().X &&
						Location.X + Item->GetSizeOnGrid().X > Items[i]->GetLocation().X &&
						Location.Y < Items[i]->GetLocation().Y + Items[i]->GetSizeOnGrid().Y &&
						Location.Y + Item->GetSizeOnGrid().Y > Items[i]->GetLocation().Y)
					{
						return false;
					}
				}
			}

			Item->RemoveSelf();
			Items.Add(Item);
			Item->Place(Location, this);

			if (bIsInstancing && LastMesh)
			{
				Item->InstanceOnMesh(LastMesh, Socket);
			}

			return true;
		}
	}

	return false;
}

UInventoryItem* UGrid::Remove_Implementation(UInventoryItem* Item)
{
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i] == Item)
		{
			UInventoryItem* ItemToRemove = Items[i];
			Items.RemoveSingle(ItemToRemove);
			ItemToRemove->DestroyInstance();
			return ItemToRemove;
		}
	}
	return nullptr;
}

void UGrid::DestroyItems_Implementation()
{
	for (int i = 0; i < Items.Num(); i++)
	{
		UInventoryItem* Item = Items[i];
		if (Item)
		{
			Item->DestroyInstance();
		}
	}

	Super::DestroyItems_Implementation();
}

TArray<UInventoryItem*> UGrid::GetItems_Implementation()
{
	return Items;
}

FVector UGrid::TransformLocation_Implementation(FPoint Location)
{
	return FVector(0, Location.X * Step, -Location.Y * Step);
}