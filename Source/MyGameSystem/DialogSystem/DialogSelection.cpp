// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogSelection.h"
#include "Dialog.h"
#include "DialogComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void UDialogSelection::Activate_Implementation(UDialog* OwnDialog)
{
	OwningDialog = OwnDialog;
	if (bHasTimer)
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UDialogSelection::SelectNextCue, 0);
		GetWorld()->GetTimerManager().SetTimer(SelectionEndTimer, Delegate, TimerDuration, false);
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
	for (TSubclassOf<UDialogCue> Cue : CueOptions)
	{
		UDialogCue* CueObject = Cast<UDialogCue>(Cue.GetDefaultObject());
		if (CueObject)
		{
			if (CueObject->CheckAvailabilityCondition(OwningDialog))
			{
				AvailableOptions.Add(Cue);
			}
		}
	}
	return AvailableOptions;
} 

FDialogSelectionStruct UDialogSelection::GetSelectionInfo()
{
	TArray<FDialogCueStruct> OptionsInfo = TArray<FDialogCueStruct>();
	for (TSubclassOf<UDialogCue> CueClass : CueOptions)
	{
		if (IsValid(CueClass))
		{
			UDialogCue* Cue = Cast<UDialogCue>(CueClass->GetDefaultObject());
			if (Cue)
			{
				if (Cue->CheckAvailabilityCondition(OwningDialog))
				{
					OptionsInfo.Add(Cue->GetCueInfo());
				}
			}
		}
	}
	return FDialogSelectionStruct(bHasTimer, GetWorld()->GetTimerManager().GetTimerRemaining(SelectionEndTimer), OptionsInfo);
}