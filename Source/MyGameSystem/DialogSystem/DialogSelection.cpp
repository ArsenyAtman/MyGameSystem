// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSelection.h"
#include "Dialog.h"
#include "DialogComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialogSelection::Activate_Implementation(UDialog* OwnDialog)
{
	OwningDialog = OwnDialog;
	if (IsValid(SelectionInfo) && SelectionInfo->bWithTimer)
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UDialogSelection::SelectNextCue, 0);
		GetWorld()->GetTimerManager().SetTimer(SelectionEndTimer, Delegate, SelectionInfo->Time, false);
	}
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

	GetWorld()->GetTimerManager().ClearTimer(SelectionEndTimer);
}

TArray<TSubclassOf<UDialogCue>> UDialogSelection::GetAvailableOptions()
{
	TArray<TSubclassOf<UDialogCue>> AvailableOptions;
	for (TSubclassOf<UDialogCue>& Cue : SelectionInfo->Options)
	{
		UDialogCue* CueObject = Cast<UDialogCue>(Cue.GetDefaultObject());
		if (IsValid(CueObject) && CueObject->CheckAvailabilityCondition(OwningDialog))
		{
			AvailableOptions.Add(Cue);
		}
	}
	return AvailableOptions;
}
