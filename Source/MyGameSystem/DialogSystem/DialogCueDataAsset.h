#pragma once

#include "CoreMinimal.h"
#include "DialogUnitDataAsset.h"
#include "DialogSystemTypes.h"
#include "DialogCueDataAsset.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogCueDataAsset : public UDialogUnitDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDialogCueType DialogCueType = EDialogCueType::Usual;
};