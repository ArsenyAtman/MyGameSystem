#pragma once

#include "CoreMinimal.h"
#include "DialogSelectionDataAsset.h"
#include "DialogSelectionDataAsset_WithTimer.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogSelectionDataAsset_WithTimer : public UDialogSelectionDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Time = -1.0f;

};