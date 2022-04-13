// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSelection.h"
#include "Dialog.h"
#include "DialogUnit.h"
#include "DialogComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DialogSelectionDataAsset.h"

void UDialogSelection::Activate_Implementation(UDialog* OwnDialog)
{
	OwningDialog = OwnDialog;
	
	OnSelectionStarted();
}

void UDialogSelection::SelectNextCue_Implementation(int CueIndex)
{
	TArray<TSubclassOf<UDialogUnit>> AvailableOptions = GetAvailableOptions();
	if (AvailableOptions.IsValidIndex(CueIndex))
	{
		OwningDialog->OnDialogUnitPassed(this, AvailableOptions[CueIndex]);
	}
	else
	{
		OwningDialog->OnDialogUnitPassed(this, nullptr);
	}

	OnSelectionEnded();
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
