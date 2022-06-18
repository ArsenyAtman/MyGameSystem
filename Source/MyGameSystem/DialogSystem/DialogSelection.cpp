// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSelection.h"
#include "Dialog.h"
#include "DialogUnit.h"
#include "DialogComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialogSelection::Activate_Implementation()
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		OnSelectionStarted();
	}
}

bool UDialogSelection::SelectNextDialogUnit_Implementation(TSubclassOf<UDialogUnit> NextDialogUnit)
{
	if(GetNetRole() == ENetRole::ROLE_Authority)
	{
		TArray<TSubclassOf<UDialogUnit>> AvailableOptions = GetAvailableOptions();
		if (AvailableOptions.Find(NextDialogUnit) != INDEX_NONE)
		{
			GetOwningDialog()->OnDialogUnitPassed(this, NextDialogUnit);
			OnSelectionEnded();
			return true;
		}
	}

	return false;
}

TArray<TSubclassOf<UDialogUnit>> UDialogSelection::GetAvailableOptions() const
{
	TArray<TSubclassOf<UDialogUnit>> AvailableOptions;
	for (const TSubclassOf<UDialogUnit>& DialogUnitClass : Options)
	{
		UDialogUnit* DialogUnitObject = Cast<UDialogUnit>(DialogUnitClass.GetDefaultObject());
		if (IsValid(DialogUnitObject) && DialogUnitObject->CheckAvailabilityCondition(GetOwningDialog()))
		{
			AvailableOptions.Add(DialogUnitClass);
		}
	}
	return AvailableOptions;
}
