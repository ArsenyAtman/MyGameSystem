#pragma once

#include "CoreMinimal.h"
#include "DialogSelectionDataAsset.h"
#include "DialogAutoSelectionDataAsset.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogAutoSelectionDataAsset : public UDialogSelectionDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCheckFromEnd = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bSelectFalseCondition = false;
};