#pragma once

#include "CoreMinimal.h"
#include "DialogSelection.h"
#include "DialogSelection_WithTimer.generated.h"


UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection_WithTimer : public UDialogSelection
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintGetter)
    float GetInitialTime() const {return InitialTime;}

protected:

    virtual void OnSelectionStarted_Implementation() override;

    virtual void OnSelectionEnded_Implementation() override;

private:

    UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetInitialTime, Category = "DialogSelection_WithTimer|Timer", meta = (AllowPrivateAccess = true))
	float InitialTime = -1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogSelection_WithTimer|Timer", meta = (AllowPrivateAccess = true))
    int DefaultOption = 0;

    FTimerHandle SelectionEndTimer;

};