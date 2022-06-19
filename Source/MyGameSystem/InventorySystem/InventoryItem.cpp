// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Instance.h"
#include "PickableInstance.h"
#include "Place.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Instance.h"

AInstance* UInventoryItem::InstanceOnMesh_Implementation(USceneComponent* Mesh, FName Socket)
{
	if (!Instance && Mesh)
	{
		Instance = GetWorld()->SpawnActorDeferred<AInstance>(InstanceClass, Mesh->GetComponentTransform());
		if (IsValid(Instance))
		{
			Instance->SetInventoryItem(this);
			Instance->GetUniversalMesh()->SetSimulatePhysics(false);
			Instance->FinishSpawning(Mesh->GetComponentTransform());
			FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			Instance->AttachToComponent(Mesh, Rules, Socket);
			Instance->SetActorRelativeLocation(OwningPlace->TransformLocation(Location));
		}
		return Instance;
	}
	return nullptr;
}

void UInventoryItem::InitializeInActor_Implementation(AInstance* InstanceActor)
{
	Instance = InstanceActor;
}

AInstance* UInventoryItem::Spawn_Implementation(FTransform Transform, bool bUseCurrentTransform)
{
	if (Instance && bUseCurrentTransform)
	{
		Transform = Instance->GetTransform();
	}

	RemoveSelf();

	Instance = GetWorld()->SpawnActorDeferred<AInstance>(PickableInstanceClass, Transform);
	if (IsValid(Instance))
	{
		Instance->SetInventoryItem(this);
		Instance->FinishSpawning(Transform);
		Instance->GetUniversalMesh()->SetSimulatePhysics(true);
	}
	return Instance;
}

void UInventoryItem::DestroyInstance_Implementation()
{
	DestroySubinstances();

	if (Instance)
	{
		Instance->Destroy();
		Instance = nullptr;
	}
}

void UInventoryItem::DestroySubinstances_Implementation()
{
	return;
}

UInventoryItem* UInventoryItem::RemoveSelf_Implementation()
{
	DestroyInstance();

	if (OwningPlace)
	{
		OwningPlace->Remove(this);
		OwningPlace = nullptr;
	}
	return this;
}

bool UInventoryItem::Place_Implementation(FPoint NewLocation, UPlace* NewOwningPlace)
{
	if (!IsValid(OwningPlace))
	{
		SetLocation(NewLocation);
		SetOwningPlace(NewOwningPlace);
		return true;
	}
	return false;
}

FPoint UInventoryItem::GetSizeOnGrid()
{
	return SizeOnGrid;
}

int UInventoryItem::GetSizeOnLine()
{
	return SizeOnLine;
}

FPoint UInventoryItem::GetLocation()
{
	return Location;
}

void UInventoryItem::SetLocation(FPoint NewLocation)
{
	Location = NewLocation;
}

void UInventoryItem::SetOwningPlace(UPlace* NewOwningPlace)
{
	OwningPlace = NewOwningPlace;
}

UPlace* UInventoryItem::GetOwningPlace()
{
	return OwningPlace;
}

UStaticMesh* UInventoryItem::GetStaticMesh()
{
	return StaticMesh;
}

USkeletalMesh* UInventoryItem::GetSkeletalMesh()
{
	return SkeletalMesh;
}
/*
AActor* UInventoryItem::GetInstance()
{
	return Instance;
}
*/

void UInventoryItem::Move(FPoint DeltaLocation)
{
	this->Location = Location + DeltaLocation;
	Instance->SetActorRelativeLocation(OwningPlace->TransformLocation(this->Location));
}

FSocketInfo UInventoryItem::GetSocketInfo(FName SocketName)
{
	FSocketInfo Socket;
	if (IsValid(Instance))
	{
		if (Instance->GetUniversalMesh()->GetMesh()->DoesSocketExist(SocketName))
		{
			Socket.Component = Instance->GetUniversalMesh()->GetMesh();
			Socket.Name = SocketName;
		}
	}
	return Socket;
}