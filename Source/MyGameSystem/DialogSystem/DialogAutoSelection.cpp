// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogAutoSelection.h"
#include "Dialog.h"
#include "DialogUnit.h"
#include "DialogAutoSelectionDataAsset.h"

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

UDialogAutoSelectionDataAsset* UDialogAutoSelection::GetDialogUnitData_Implementation() const
{
	return Cast<UDialogAutoSelectionDataAsset>(Super::GetDialogUnitData_Implementation());
}