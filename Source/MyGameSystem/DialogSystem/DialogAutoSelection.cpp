// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAutoSelection.h"
#include "Dialog.h"
#include "DialogCue.h"
#include "DialogAutoSelectionDataAsset.h"

void UDialogAutoSelection::Activate_Implementation(UDialog* OwnDialog)
{
	TArray<TSubclassOf<UDialogCue>> DialogCueOptions = GetDialogUnitData_Implementation()->Options;
	if (bCheckFromEnd)
	{
		Algo::Reverse(DialogCueOptions);
	}

	for (const TSubclassOf<UDialogCue>& DialogCueClass : DialogCueOptions)
	{
		if (IsValid(DialogCueClass))
		{
			UDialogCue* DialogCueObject = Cast<UDialogCue>(DialogCueClass->GetDefaultObject());
			if (IsValid(DialogCueObject))
			{
				bool bConditionResult = DialogCueObject->CheckAvailabilityCondition(OwnDialog);
				if (bSelectFalseCondition)
				{
					bConditionResult = !bConditionResult;
				}

				if (bConditionResult)
				{
					OwnDialog->OnDialogUnitPassed(this, DialogCueClass);
					return;
				}
			}
			else
			{
				OwnDialog->OnDialogUnitPassed(this, DialogCueClass);
				return;
			}
		}
	}
	OwnDialog->OnDialogUnitPassed(this, nullptr);
}

UDialogAutoSelectionDataAsset* UDialogAutoSelection::GetDialogUnitData_Implementation() const
{
	return Cast<UDialogAutoSelectionDataAsset>(Super::GetDialogUnitData_Implementation());
}