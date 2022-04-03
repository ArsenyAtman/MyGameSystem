#pragma once

#include "CoreMinimal.h"
#include "DialogUnitDataAsset.h"
#include "DialogSelectionDataAsset.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogSelectionDataAsset : public UDialogUnitDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<class UDialogCue>> Options;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bWithTimer = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Time = -1.0f;

};