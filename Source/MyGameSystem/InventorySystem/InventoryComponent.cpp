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
	PrimaryComponentTick.bCanEverTick = false;

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

		InventoryItem->Spawn(Transform, true);
		return true;
	}
	return false;
}
