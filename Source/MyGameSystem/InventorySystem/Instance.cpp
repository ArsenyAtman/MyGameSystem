// Fill out your copyright notice in the Description page of Project Settings.

#include "Instance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniversalMeshComponent.h"
#include "InventoryItem.h"

AInstance::AInstance()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AInstance::Destroyed()
{
	if (InventoryItem)
	{
		InventoryItem->DestroyInstance();
	}

	Super::Destroyed();
}

#if WITH_EDITOR
void AInstance::PreEditChange(FProperty* PropertyAboutToChange)
{
	if (PropertyAboutToChange)
	{
		if (PropertyAboutToChange->NamePrivate == "InventoryItem")
		{
			if (InventoryItem)
			{
				InventoryItem->DestroySubinstances();
			}
		}
	}
}
#endif