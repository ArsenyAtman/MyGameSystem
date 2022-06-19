// Fill out your copyright notice in the Description page of Project Settings.

#include "ComplexItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniversalMeshComponent.h"
#include "Instance.h"

void UComplexItem::InitializeInActor_Implementation(AInstance* InstanceActor)
{
	Super::InitializeInActor_Implementation(InstanceActor);

	if (IsValid(GetInstance()))
	{
		InstancePlaces(GetInstance()->GetUniversalMesh()->GetMesh());
	}
}

void UComplexItem::DestroySubinstances_Implementation()
{
	for (int i = 0; i < Places.Num(); i++)
	{
		if (Places[i])
		{
			Places[i]->DestroyItems();
		}
	}

	Super::DestroySubinstances_Implementation();
}

void UComplexItem::InstancePlaces_Implementation(USceneComponent* Mesh)
{
	for (int i = 0; i < Places.Num(); i++)
	{
		if (Places[i])
		{
			Places[i]->InstanceItems(Mesh);
		}
	}
}

TArray<UPlace*> UComplexItem::GetPlaces_Implementation()
{
	return Places;
}