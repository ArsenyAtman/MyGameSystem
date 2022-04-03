#pragma once

#include "CoreMinimal.h"
#include "DialogCueDataAsset.h"
#include "DialogCueDataAsset_AnimationAndSound.generated.h"

UCLASS()
class MYGAMESYSTEM_API UDialogCueDataAsset_AnimationAndSound : public UDialogCueDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* Animation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueVoice* Voice = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDialogueWave* Sound = nullptr;
};