#include "DialogSelection_WithTimer.h"

UDialogSelectionDataAsset_WithTimer* UDialogSelection_WithTimer::GetDialogUnitData_Implementation()
{
    return Cast<UDialogSelectionDataAsset_WithTimer>(Super::GetDialogUnitData_Implementation());
}

void UDialogSelection_WithTimer::OnSelectionStarted_Implementation()
{
    Super::OnSelectionStarted_Implementation();

    if (IsValid(GetDialogUnitData_Implementation()))
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UDialogSelection::SelectNextCue, GetDialogUnitData_Implementation()->DefaultOption);
		GetWorld()->GetTimerManager().SetTimer(SelectionEndTimer, Delegate, GetDialogUnitData_Implementation()->Time, false);
	}
}

void UDialogSelection_WithTimer::OnSelectionEnded_Implementation()
{
    Super::OnSelectionEnded_Implementation(); 

    GetWorld()->GetTimerManager().ClearTimer(SelectionEndTimer);
}