// Fill out your copyright notice in the Description page of Project Settings.

#include "PickableInstance.h"
#include "Instance.h"
#include "Kismet/KismetSystemLibrary.h"


UInventoryItem* APickableInstance::PickUp_Implementation()
{
	GetInventoryItem()->DestroyInstance();
	return GetInventoryItem();
}

void APickableInstance::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(VelocityCheckTimer, this, &APickableInstance::VelocityCheck, 2.0f, false);
}

void APickableInstance::VelocityCheck()
{
	if (this->GetVelocity().Size() <= 10.0f)
	{
		this->GetUniversalMesh()->SetSimulatePhysics(false);
		GetWorldTimerManager().ClearTimer(VelocityCheckTimer);
		UKismetSystemLibrary::PrintString(GetWorld(), "STOP SimPh");
	}
	else
	{
		GetWorldTimerManager().SetTimer(VelocityCheckTimer, this, &APickableInstance::VelocityCheck, 2.0f, false);
	}
}