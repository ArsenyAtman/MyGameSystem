// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSelection.h"
#include "Dialog.h"
#include "DialogUnit.h"
#include "DialogComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialogSelection::Activate_Implementation()
{
	if(GetNetRole() == ENetRole::Role_Authority)
	{
		OnSelectionStarted();
	}
}

void UDialogSelection::SelectNextCue_Implementation(int CueIndex)
{
	if(GetNetRole() == ENetRole::Role_Authority)
	{
		TArray<TSubclassOf<UDialogUnit>> AvailableOptions = GetAvailableOptions();
		if (AvailableOptions.IsValidIndex(CueIndex))
		{
			GetOwningDialog()->OnDialogUnitPassed(this, AvailableOptions[CueIndex]);
		}
		else
		{
			GetOwningDialog()->OnDialogUnitPassed(this, nullptr);
		}

		OnSelectionEnded();
	}
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
