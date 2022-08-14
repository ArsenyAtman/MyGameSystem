#pragma once

#include "CoreMinimal.h"
#include "DialogSelection.h"
#include "DialogSelection_WithTimer.generated.h"

/**
 * DialogSelection that automatically selects a predefined option after the timer expires.
 */
UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection_WithTimer : public UDialogSelection
{
	GENERATED_BODY()

public:

    /**
     * Get the timer duration.
     * @return The timer duration.
     */
    UFUNCTION(BlueprintGetter, Category = "DialogSelection_WithTimer|Timer")
    float GetInitialTime() const {return InitialTime;}

protected:

    // Override for the timer setup.
    virtual void OnSelectionStarted_Implementation() override;

    // Override for the timer cleanup.
    virtual void OnSelectionEnded_Implementation() override;

private:

    /**
     * The duration value of the timer.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetInitialTime, Category = "DialogSelection_WithTimer|Timer", meta = (AllowPrivateAccess = true))
	float InitialTime = -1.0f;

    /**
     * The index of an option to select.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogSelection_WithTimer|Timer", meta = (AllowPrivateAccess = true))
    TSubclassOf<class UDialogUnit> DefaultOption = nullptr;

    FTimerHandle SelectionEndTimer;

};