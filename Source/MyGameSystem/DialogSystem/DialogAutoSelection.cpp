// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAutoSelection.h"
#include "Dialog.h"

void UDialogAutoSelection::Activate_Implementation(UDialog* OwnDialog)
{
	TArray<TSubclassOf<UObject>> DialogCueOptions = CueOptions;
	if (bCheckFromEnd)
	{
		Algo::Reverse(DialogCueOptions);
	}
	for (int i = 0; i < DialogCueOptions.Num(); i++)
	{
		if (IsValid(DialogCueOptions[i]))
		{
			UDialogCue* DialogCueObject = Cast<UDialogCue>(DialogCueOptions[i]->GetDefaultObject());
			if (IsValid(DialogCueObject))
			{
				bool bConditionResult = DialogCueObject->CheckAvailabilityCondition(OwnDialog);
				if (bSelectFalseCondition)
				{
					bConditionResult = !bConditionResult;
				}

				if (bConditionResult)
				{
					OwnDialog->OnDialogUnitPassed(this, DialogCueOptions[i]);
					return;
				}
			}
			else
			{
				OwnDialog->OnDialogUnitPassed(this, DialogCueOptions[i]);
				return;
			}
		}
	}
	OwnDialog->OnDialogUnitPassed(this, nullptr);
}