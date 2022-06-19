// Fill out your copyright notice in the Description page of Project Settings.


#include "Place.h"
#include "InventoryItem.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"

bool UPlace::Add_Implementation(UInventoryItem* Item, FPoint Location)
{
	return false;
}

UInventoryItem* UPlace::Remove_Implementation(UInventoryItem* Item)
{
	return nullptr;
}

void UPlace::InstanceItems_Implementation(USceneComponent* Mesh)
{
	LastMesh = Mesh;
		
	SetOwnerToItems();

	TArray<UInventoryItem*> Items = this->GetItems();
	if (bIsInstancing && IsValid(LastMesh))
	{
		for (int i = 0; i < Items.Num(); i++)
		{
			UInventoryItem* Item = Items[i];
			if (Item)
			{
				Item->InstanceOnMesh(Mesh, Socket);
			}
		}
	}
}

void UPlace::DestroyItems_Implementation()
{
	LastMesh = nullptr;
}

TArray<UInventoryItem*> UPlace::GetItems_Implementation()
{
	return TArray<UInventoryItem*>();
}

void UPlace::SetOwnerToItems()
{
	TArray<UInventoryItem*> Items = this->GetItems();
	for (int i = 0; i < Items.Num(); i++)
	{
		UInventoryItem* Item = Items[i];
		if (Item)
		{
			Item->SetOwningPlace(this);
			UKismetSystemLibrary::PrintString(GetWorld(), Item->GetName());
		}
	}
}

FVector UPlace::TransformLocation_Implementation(FPoint Location)
{
	return FVector::ZeroVector;
}

bool UPlace::CheckOnCompability(UInventoryItem* Item)
{
	if (Item)
	{
		for (int i = 0; i < ForSubclassesOf.Num(); i++)
		{
			if (Item->GetClass()->IsChildOf(ForSubclassesOf[i]))
			{
				return true;
			}
		}
	}
	
	return false;
}