#pragma once

#include "CoreMinimal.h"
#include "DialogSelection.h"
#include "DialogSelectionDataAsset_WithTimer.h"
#include "DialogSelection_WithTimer.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MYGAMESYSTEM_API UDialogSelection_WithTimer : public UDialogSelection
{
	GENERATED_BODY()

public:

    virtual class UDialogSelectionDataAsset_WithTimer* GetDialogUnitData_Implementation() const override;

protected:

    virtual void OnSelectionStarted_Implementation() override;

    virtual void OnSelectionEnded_Implementation() override;

private:

    FTimerHandle SelectionEndTimer;

};