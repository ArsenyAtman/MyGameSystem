// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniversalMeshComponent.h"
#include "Container.h"


UContainerItem::UContainerItem()
{

}

void UContainerItem::Add_Implementation(UInventoryItem* Item)
{
	Container->Add(Item, FPoint());
}

UInventoryItem* UContainerItem::Pop_Implementation()
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Pop", true, true, FLinearColor::Red);
	return Container->Remove();
}

UInventoryItem* UContainerItem::GetLastItem_Implementation()
{
	return Container->GetLastItem();
}

void UContainerItem::InitializeInActor_Implementation(AInstance* InstanceActor)
{
	Super::InitializeInActor_Implementation(InstanceActor);

	if (IsValid(GetInstance()))
	{
		Container->InstanceItems(GetInstance()->GetUniversalMesh()->GetMesh());
	}
}