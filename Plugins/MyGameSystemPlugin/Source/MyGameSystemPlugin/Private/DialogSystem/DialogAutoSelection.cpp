// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogSystem/DialogAutoSelection.h"

#include "DialogSystem/Dialog.h"
#include "DialogSystem/DialogUnit.h"

void UDialogAutoSelection::OnSelectionStarted_Implementation()
{
	TArray<TSubclassOf<UDialogUnit>> DialogUnitOptions = GetAvailableOptions();
	if (bCheckFromEnd)
	{
		Algo::Reverse(DialogUnitOptions);
	}

	for (const TSubclassOf<UDialogUnit>& DialogUnitClass : DialogUnitOptions)
	{
		if (IsValid(DialogUnitClass))
		{
			UDialogUnit* DialogUnitObject = Cast<UDialogUnit>(DialogUnitClass->GetDefaultObject());
			if (IsValid(DialogUnitObject))
			{
				bool bConditionResult = DialogUnitObject->CheckAvailabilityCondition(GetOwningDialog());
				if (bSelectFalseCondition)
				{
					bConditionResult = !bConditionResult;
				}

				if (bConditionResult)
				{
					GetOwningDialog()->OnDialogUnitPassed(this, DialogUnitClass);
					return;
				}
			}
			else
			{
				GetOwningDialog()->OnDialogUnitPassed(this, DialogUnitClass);
				return;
			}
		}
	}
	GetOwningDialog()->OnDialogUnitPassed(this, nullptr);
}
