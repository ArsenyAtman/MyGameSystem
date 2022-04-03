#pragma once

#include "CoreMinimal.h"
#include "DialogUnitDataAsset.h"
#include "DialogAutoSelectionDataAsset.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelectionDataAsset : public UDialogUnitDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class UDialogCue>> CueOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCheckFromEnd = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSelectFalseCondition = false;
};