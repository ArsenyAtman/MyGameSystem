#include "DialogSelection_WithTimer.h"

UDialogSelectionDataAsset_WithTimer* UDialogSelection_WithTimer::GetDialogUnitData_Implementation() const
{
    return Cast<UDialogSelectionDataAsset_WithTimer>(Super::GetDialogUnitData_Implementation());
}

void UDialogSelection_WithTimer::OnSelectionStarted_Implementation()
{
    Super::OnSelectionStarted_Implementation();

    if (Time > 0.0f)
	{
		FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UDialogSelection::SelectNextCue, DefaultOption);
		GetWorld()->GetTimerManager().SetTimer(SelectionEndTimer, Delegate, Time, false);
	}
}

void UDialogSelection_WithTimer::OnSelectionEnded_Implementation()
{
    Super::OnSelectionEnded_Implementation(); 

    GetWorld()->GetTimerManager().ClearTimer(SelectionEndTimer);
}