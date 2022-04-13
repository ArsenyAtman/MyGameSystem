// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSelection.h"
#include "Dialog.h"
#include "DialogCue.h"
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
	TArray<TSubclassOf<UDialogCue>> AvailableOptions = GetAvailableOptions();
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

TArray<TSubclassOf<UDialogCue>> UDialogSelection::GetAvailableOptions() const
{
	TArray<TSubclassOf<UDialogCue>> AvailableOptions;
	for (const TSubclassOf<UDialogCue>& Cue : Options)
	{
		UDialogCue* CueObject = Cast<UDialogCue>(Cue.GetDefaultObject());
		if (IsValid(CueObject) && CueObject->CheckAvailabilityCondition(OwningDialog))
		{
			AvailableOptions.Add(Cue);
		}
	}
	return AvailableOptions;
}
