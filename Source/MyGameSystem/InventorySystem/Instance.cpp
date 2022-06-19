// Fill out your copyright notice in the Description page of Project Settings.

#include "Instance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniversalMeshComponent.h"
#include "InventoryItem.h"

AInstance::AInstance()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	RootComponent = DefaultRoot;

	UniversalMesh = CreateDefaultSubobject<UUniversalMeshComponent>(TEXT("Mesh"));
	UniversalMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	DummyMesh = SphereMeshAsset.Object;
}

void AInstance::OnConstruction(const FTransform& Transform)
{
	if (InventoryItem)
	{
		if (InventoryItem->GetStaticMesh())
		{
			UniversalMesh->SetStaticMesh(InventoryItem->GetStaticMesh());
		}
		else if (InventoryItem->GetSkeletalMesh())
		{
			UniversalMesh->SetSkeletalMesh(InventoryItem->GetSkeletalMesh());
		}
		else
		{
			UniversalMesh->ClearMesh();
		}

		InventoryItem->InitializeInActor(this);
	}
	else
	{
		UniversalMesh->SetStaticMesh(DummyMesh);
	}
}

void AInstance::BeginPlay()
{
	Super::BeginPlay();
}

void AInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UUniversalMeshComponent* AInstance::GetUniversalMesh()
{
	return UniversalMesh;
}

void AInstance::Destroyed()
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Destroyed");

	if (InventoryItem)
	{
		InventoryItem->DestroyInstance();
	}

	Super::Destroyed();
}

UInventoryItem* AInstance::GetInventoryItem()
{
	return InventoryItem;
}

void AInstance::SetInventoryItem(UInventoryItem* NewInventoryItem)
{
	InventoryItem = NewInventoryItem;
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