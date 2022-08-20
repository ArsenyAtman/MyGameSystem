
#include "DialogSelection_WithTimer.h"
#include "Net/UnrealNetwork.h"


void UDialogSelection_WithTimer::OnSelectionStarted_Implementation()
{
    Super::OnSelectionStarted_Implementation();

    if (HasAuthority())
    {
        if (InitialTime > 0.0f)
        {
            FTimerDelegate Delegate;// = FTimerDelegate::CreateUObject(this, &UDialogSelection::SelectNextDialogUnit, DefaultOption);
            Delegate.BindUFunction(this, TEXT("SelectNextDialogUnit"), DefaultOption);
            GetWorld()->GetTimerManager().SetTimer(SelectionEndTimer, Delegate, InitialTime, false);
        }
    }
}

void UDialogSelection_WithTimer::OnSelectionEnded_Implementation()
{
    if (HasAuthority())
    {
        GetWorld()->GetTimerManager().ClearTimer(SelectionEndTimer);
    }

    Super::OnSelectionEnded_Implementation(); 
}
